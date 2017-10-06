# include <kernel/user.h>
# include "Interface.h"
# include "User.h"
# include "Packet.h"
# include "RandSeq.h"

inherit Interface;

/*
 * ACK is sent when new seq received, at most once per 2 secs.  Timer resets
 * when retransmit request is sent.  When no further data included, ACK
 * reuses last seqno, checksum not encrypted.  Same for ReqRetrans with no
 * further data.
 *
 * Retransmit request is sent solo ~50 millisecs after missing packet detected;
 * depending on detected round-trip speed? Echo req/resp timing is as low as
 * 10 millisecs, so retrans req has a timer?  It also tries to hike on next
 * packet, like ACK.  2nd retrans req is sent after ~750 millisecs.
 * After receiving retransmission request, answer is sent immediately.
 *
 * RejectRetransmit is used when a retransmit request is received for a
 * packet that was not retained.  RejectRetransmit can be observed in the pcaps,
 * so it could be used to skip retransmitting non-essential packets.
 *
 * Echo response can be delayed for a while, hikes on next packet?  Echo request
 * always does, echoResponse sometimes (rarely, overflow situation?) goes alone.
 * TimeSync is never alone.  Flow is often alone.
 *
 * In portal-storm situations, it is the server which is losing input packets,
 * meaning that the input queue fills up faster than the server can digest
 * them, and packets are discarded.
 */

static void transmit(Packet packet, int required);
static int retransmit(int sequence);
static void acknowledged(int sequence);


/* ========================================================================= *
 *			packet output buffer				     *
 * ========================================================================= */

# define MAX_BUF_SIZE		464
# define FRAG_HEADER_SIZE	16
# define MAX_FRAG_SIZE		(MAX_BUF_SIZE - FRAG_HEADER_SIZE)
# define FRAG_ID		0x03000000

private int serverId;		/* ID of the server */
private Packet bufPacket;	/* buffered partial packet */
private int bufTime;		/* buf start time */
private float bufMTime;		/* buf start millitime */
private int bufSize;		/* packet buffer space used */
private int bufRequired;	/* is current buffered packet required? */
private int pendingDrain;	/* buffer drain callout active? */
private int serverFrag;		/* seq of last fragment sent */

/*
 * create packet output buffer
 */
private void bufCreate(int serverId)
{
    ::serverId = serverId;
}

/*
 * start a new packet output buffer
 */
private void bufStart()
{
    bufPacket = new Packet(0, serverId, 0);
    ({ bufTime, bufMTime }) = millitime();
    bufRequired = FALSE;
    if (pendingDrain == 0) {
	pendingDrain = call_out("bufDrain", 0.005, bufPacket);
    }
}

/*
 * flush the packet output buffer
 */
private void bufFlush()
{
    if (bufPacket) {
	bufPacket->setEncryptedChecksum();
	transmit(bufPacket, bufRequired);
	bufPacket = nil;
	bufSize = 0;
    }
}

/*
 * drain the packet output buffer after 5 millisecs
 */
static void bufDrain(Packet packet)
{
    if (packet == bufPacket) {
	bufFlush();
    } else if (bufPacket) {
	float timePassed;

	timePassed = timeDiff(bufTime, bufMTime, millitime()...);
	if (timePassed >= 0.005) {
	    bufFlush();
	} else {
	    pendingDrain = call_out("bufDrain", 0.005 - timePassed, bufPacket);
	    return;
	}
    }

    pendingDrain = 0;
}

/*
 * add optional header to output packet
 */
private void bufData(NetworkData data, int required)
{
    int size;

    if (!bufPacket || !(bufPacket->flags() & data->networkDataType())) {
	size = data->size();
	if (bufSize + size > MAX_BUF_SIZE) {
	    bufFlush();
	}
    } else {
	size = 0;	/* replace existing header */
    }
    if (!bufPacket) {
	bufStart();
    }
    bufPacket->addData(data);
    bufRequired |= required;

    bufSize += size;
    if (bufSize == MAX_BUF_SIZE) {
	bufFlush();
    }
}

/*
 * fragment message across output packets
 */
private void bufMessage(string message, int group, int required)
{
    int count, i, len;
    Fragment fragment;

    if (bufSize >= MAX_FRAG_SIZE) {
	bufFlush();
    }
    if (!bufPacket) {
	bufStart();
    }

    serverFrag++;
    count = 1;
    i = 0;
    len = strlen(message);
    if (bufSize + len > MAX_FRAG_SIZE) {
	count += (bufSize + len - 1) / MAX_FRAG_SIZE;
	len = MAX_FRAG_SIZE - bufSize;

	fragment = new Fragment(serverFrag, serverFrag | FRAG_ID, count, i,
				group, message[.. len - 1]);
	bufPacket->addFragment(fragment);
	bufRequired |= required;
	bufFlush();
	message = message[len ..];

	while (++i < count - 1) {
	    bufStart();
	    fragment = new Fragment(serverFrag, serverFrag | FRAG_ID, count, i,
				    group, message[.. MAX_FRAG_SIZE - 1]);
	    bufPacket->addFragment(fragment);
	    bufRequired = required;
	    bufFlush();
	    message = message[MAX_FRAG_SIZE ..];
	}

	bufStart();
    }

    fragment = new Fragment(serverFrag, serverFrag | FRAG_ID, count, i, group,
			    message);
    bufPacket->addFragment(fragment);
    bufRequired |= required;

    bufSize += FRAG_HEADER_SIZE + strlen(message);
    if (bufSize == MAX_BUF_SIZE) {
	bufFlush();
    }
}

/*
 * send a message to the client
 */
static void bufSend(string message, int group, int required)
{
    bufMessage(message, group, required);
}


/* ========================================================================= *
 *			client message handling				     *
 * ========================================================================= */

private int clientId;		/* session ID */
private RandSeq clientRand;	/* for checksum encryption */
private mapping clientQueue;	/* packet queue */
private mapping fragmentQueue;	/* fragment queue */
private int clientSeq;		/* seq last processed */
private int ackSeq;		/* acknowledged client packet seq */
private int pendingAck;		/* scheduled ack */
private User user;		/* connected account */
private string loginError;	/* error during login */
private int receiveSeq;		/* highest seq received */
private int flowSize;		/* client packet bytes during flowTime */
private int flowTime;		/* current client packet time */

/*
 * request the client to retransmit some packets
 */
private void requestRetransmit(int *sequences)
{
    int size;
    Packet packet;

    while (!bufPacket || bufSize > MAX_BUF_SIZE - 4) {
	/* bypass the packet buffer */
	packet = new Packet(0, serverId, 0);
	if (size > MAX_BUF_SIZE / 4 - 1) {
	    size = MAX_BUF_SIZE / 4 - 1;
	}
	packet->addData(new RequestRetransmit(sequences[.. size - 1]));
	transmit(packet, FALSE);
	sequences = sequences[size ..];
    }

    if (size != 0) {
	bufData(new RequestRetransmit(sequences), FALSE);
	bufFlush();
    }

    remove_call_out(pendingAck);
    pendingAck = call_out("ack", 2.0);
}

/*
 * reject a retransmission request from the client
 */
private void rejectRetransmit(int *sequences)
{
    int size;
    Packet packet;

    while (!bufPacket || bufSize > MAX_BUF_SIZE - 4) {
	/* bypass the packet buffer */
	packet = new Packet(0, serverId, 0);
	if (size > MAX_BUF_SIZE / 4 - 1) {
	    size = MAX_BUF_SIZE / 4 - 1;
	}
	packet->addData(new RejectRetransmit(sequences[.. size - 1]));
	transmit(packet, FALSE);
	sequences = sequences[size ..];
    }

    if (size != 0) {
	if (!bufPacket) {
	    bufStart();
	}
	bufData(new RejectRetransmit(sequences), FALSE);
	bufFlush();
    }
}

/*
 * send a new AckSequence every 2 seconds
 */
static void ack()
{
    Packet packet;

    pendingAck = call_out("ack", 2.0);
    ackSeq = clientSeq;
    if (!bufPacket || bufSize > MAX_BUF_SIZE - 4) {
	/* bypass the packet buffer */
	packet = new Packet(0, serverId, 0);
	packet->addData(new AckSequence(ackSeq));
	transmit(packet, FALSE);
    } else {
	bufData(new AckSequence(ackSeq), FALSE);
	bufFlush();
    }
}

/*
 * send a new TimeSynch every 20 seconds
 */
static void sync()
{
    call_out("sync", 20);
    bufData(new TimeSynch(0, 0.0), TRUE);
}

/*
 * receive packet from client in sequence
 */
private void clientPacket(Packet packet, int sequence)
{
    int flags;

    for (;;) {
	clientSeq = sequence;

	/*
	 * process options
	 */
	flags = packet->flags();
	/* XXX timeSynch */
	/* XXX flow */
	if (flags & PACKET_ECHO_REQUEST) {
	    float clientTime;

	    clientTime = packet->data(PACKET_ECHO_REQUEST)->clientTime();
	    bufData(new EchoResponse(0.0, clientTime), TRUE);
	}

	/*
	 * process fragments
	 */
	if (flags & PACKET_BLOB_FRAGMENTS) {
	    int n, i, count;
	    Fragment fragment;
	    mapping blobs;

	    n = packet->numFragments();
	    for (i = 0; i < n; i++) {
		fragment = packet->fragment(i);
		count = fragment->count();
		if (count == 1) {
		    user->receiveMessage(fragment->blob(), fragment->group());
		} else {
		    sequence = fragment->sequence();
		    blobs = fragmentQueue[sequence];
		    if (!blobs) {
			blobs = fragmentQueue[sequence] = ([ ]);
		    }
		    blobs[fragment->index()] = fragment->blob();
		    if (map_sizeof(blobs) == count) {
			fragmentQueue[sequence] = nil;
			user->receiveMessage(implode(map_values(blobs), ""),
					     fragment->group());
		    }
		}
	    }
	}

	sequence = clientSeq + 1;
	packet = clientQueue[sequence];
	if (!packet) {
	    return;
	}
	clientQueue[sequence] = nil;
    }
}

/*
 * login during 3-way handshake
 */
static int login(string name, string password, int serverId, int clientId,
		 int clientSeed)
{
    string message;

    catch {
	({ user, message }) = ::login(name, password);
	if (!user) {
	    if (message) {
		loginError = message;
	    } else {
		return FALSE;	/* no response whatsoever */
	    }
	}
    } : {
	return FALSE;		/* error during login: disconnect immediately */
    }

    bufCreate(serverId);
    ::clientId = clientId;
    clientRand = new RandSeq(clientSeed);
    ackSeq = clientSeq = 1;
    clientQueue = ([ ]);
    fragmentQueue = ([ ]);

    return TRUE;
}

/*
 * connection was established
 */
static void establish()
{
    if (loginError) {
	bufMessage(loginError, 9, TRUE);
	bufFlush();
    } else {
	sync();
	pendingAck = call_out("ack", 2.0);
	user->establishConnection();
    }
}

/*
 * logout
 */
static void logout()
{
    if (user) {
	::logout(user);
    }
}

/*
 * packet received from the client
 */
static int receivePacket(string str)
{
    Packet packet;
    int sequence, flags, time;

    catch {
	packet = new ClientPacket(str);
    } : {
	return MODE_NOCHANGE;		/* ignore bad packets */
    }

    /*
     * sanity check
     */
    if (packet->id() != clientId) {
	/*
	 * stray packet, or CICMD_COMMAND which has clientId 0
	 */
	return MODE_NOCHANGE;
    }

    /*
     * sequence, flags, checksum
     */
    sequence = packet->sequence();
    if (sequence > clientSeq + 512) {
	return MODE_NOCHANGE;		/* too far ahead */
    }
    flags = packet->flags();
    if (flags & PACKET_ENCRYPTED_CHECKSUM) {
	if (sequence <= clientSeq) {
	    return MODE_NOCHANGE;	/* duplicate with encrypted checksum */
	}
	packet->setXorValue(clientRand->rand(sequence - 2));
    } else if (flags & ~(PACKET_REQUEST_RETRANSMIT | PACKET_REJECT_RETRANSMIT |
			 PACKET_ACK_SEQUENCE | PACKET_DISCONNECT |
			 PACKET_CONNECT_ERROR | PACKET_CICMD_COMMAND)) {
	return MODE_NOCHANGE;		/* missing encrypted checksum */
    } else if (sequence == 0) {
	if (flags & ~(PACKET_DISCONNECT | PACKET_CICMD_COMMAND)) {
	    return MODE_NOCHANGE;	/* missing sequence */
	}
    } else if (sequence <= clientSeq - 10) {
	return MODE_NOCHANGE;		/* too far back */
    }
    if (packet->checksum() != packet->computeChecksum()) {
	return MODE_NOCHANGE;		/* corrupted or different session */
    }

    switch (flags) {
    case PACKET_DISCONNECT:
    case PACKET_CONNECT_CLOSE:
	return MODE_DISCONNECT;

    case PACKET_CONNECT_ERROR:
    case PACKET_CICMD_COMMAND:
	return MODE_NOCHANGE;
    }

    /*
     * handle urgent requests out of order
     */
    if (flags & PACKET_REQUEST_RETRANSMIT) {
	int sz, i, *sequences;

	sequences = packet->data(PACKET_REQUEST_RETRANSMIT)->sequences();
	for (sz = sizeof(sequences), i = 0; i < sz; i++) {
	    if (retransmit(sequences[i])) {
		sequences[i] = 0;
	    }
	}
	sequences -= ({ 0 });
	if (sizeof(sequences) != 0) {
	    rejectRetransmit(sequences);
	}
    }
    if (flags & PACKET_REJECT_RETRANSMIT) {
	int clientSequence, sz, i, *sequences;

	sequences = packet->data(PACKET_REJECT_RETRANSMIT)->sequences();
	for (sz = sizeof(sequences), i = 0; i < sz; i++) {
	    clientSequence = sequences[i];
	    if (clientSequence > clientSeq && clientSequence <= receiveSeq &&
		!clientQueue[clientSequence]) {
		/* inject fake packet to fill the gap */
		clientQueue[clientSequence] = new Packet(0, clientId, 0);
	    }
	}
    }
    if (flags & PACKET_ACK_SEQUENCE) {
	acknowledged(packet->data(PACKET_ACK_SEQUENCE)->sequence());
    }

    /*
     * generate flow
     */
    time = packet->time();
    if (time == flowTime) {
	flowSize += packet->size();
    } else if (((time - flowTime) & 0xffff) <= 0x7fff) {
	if (flowSize != 0) {
	    if (bufPacket && (bufPacket->flags() & PACKET_FLOW)) {
		bufFlush();
	    }
	    bufData(new Flow(flowSize, flowTime), TRUE);
	}
	flowSize = packet->size();
	flowTime = time;
    }

    /*
     * process packets in sequence
     */
    if (receiveSeq < sequence) {
	receiveSeq = sequence;
    }
    if (sequence == clientSeq + 1) {
	clientPacket(packet, sequence);
    } else {
	if (flags & PACKET_ENCRYPTED_CHECKSUM) {
	    clientQueue[sequence] = packet;
	    /* XXX request up to sequence - 1 */
	} else {
	    /* XXX request up to sequence */
	}
    }

    return MODE_NOCHANGE;
}

/*
 * send a message from the user
 */
void sendMessage(string message, int group, int required)
{
    if (previous_object() == user) {
	call_out("bufSend", 0, message, group, required);
    }
}


User user()	{ return user; }
int clientId()	{ return clientId; }
