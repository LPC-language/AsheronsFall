# include <kernel/user.h>
# include <Time.h>
# include "RandSeq.h"
# include "Packet.h"
# include "Interface.h"
# include "User.h"

inherit Interface;

static void transmitPackets(Packet *packets);
static void transmitPrioData(NetworkData data);
static int retransmitPacket(int sequence);
static void acknowledgedSeq(int sequence);


/* ========================================================================= *
 *			packet output buffer				     *
 * ========================================================================= */

# define MAX_BUF_SIZE		464
# define FRAG_HEADER_SIZE	16
# define MAX_FRAG_SIZE		(MAX_BUF_SIZE - FRAG_HEADER_SIZE)
# define FRAG_ID		0x03000000

private int serverId;		/* ID of the server */
private Packet bufPacket;	/* buffered partial packet */
private Packet *bufFragments;	/* buffered fragments */
private Time bufTime;		/* buf start time */
private int bufSize;		/* packet buffer space used */
private int pendingDrain;	/* buffer drain callout active? */
private int serverFrag;		/* seq of last fragment sent */

/*
 * create packet output buffer
 */
private void bufCreate(int serverId)
{
    ::serverId = serverId;
    bufFragments = ({ });
}

/*
 * start a new packet output buffer
 */
private void bufStart()
{
    bufPacket = new Packet(0, serverId, 0);
    bufTime = new Time(millitime()...);
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
	transmitPackets(({ bufPacket }) + bufFragments);
	bufPacket = nil;
	bufSize = 0;
	bufFragments = ({ });
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

	timePassed = timeDiff(bufTime, new Time(millitime()...));
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
    if (required) {
	bufPacket->setRequired();
    }

    bufSize += size;
    if (bufSize == MAX_BUF_SIZE) {
	bufFlush();
    }
}

/*
 * fragment message across output packets
 */
static void bufMessage(string message, int group, int required)
{
    int count, i, len;
    Packet packet;

    serverFrag++;
    count = 1;
    i = 0;
    len = strlen(message);
    if (bufSize + len > MAX_FRAG_SIZE) {
	count += (len - 1) / MAX_FRAG_SIZE;

	/* fragments that fill a whole packet */
	while (len >= MAX_FRAG_SIZE) {
	    packet = new Packet(0, serverId, 0);
	    packet->addFragment(new Fragment(serverFrag, serverFrag | FRAG_ID,
					     count, i++, group,
					     message[.. MAX_FRAG_SIZE - 1]));
	    if (required) {
		packet->setRequired();
	    }
	    bufFragments += ({ packet });
	    message = message[MAX_FRAG_SIZE ..];
	    len -= MAX_FRAG_SIZE;
	}
	if (!bufPacket) {
	    transmitPackets(bufFragments);
	    bufFragments = ({ });
	}
    }

    if (len != 0) {
	if (bufSize + len > MAX_FRAG_SIZE) {
	    bufFlush();
	}
	if (!bufPacket) {
	    bufStart();
	}

	/* last fragment */
	bufPacket->addFragment(new Fragment(serverFrag, serverFrag | FRAG_ID,
					    count, i, group, message));
	if (required) {
	    bufPacket->setRequired();
	}

	bufSize += FRAG_HEADER_SIZE + len;
	if (bufSize == MAX_BUF_SIZE) {
	    bufFlush();
	}
    }
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
private User user;		/* connected user */
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

    while ((size=sizeof(sequences)) > MAX_BUF_SIZE / 4 - 1) {
	size = MAX_BUF_SIZE / 4 - 1;
	transmitPrioData(new RequestRetransmit(sequences[.. size - 1]));
	sequences = sequences[size ..];
    }

    if (size != 0) {
	transmitPrioData(new RequestRetransmit(sequences));
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

    while ((size=sizeof(sequences)) > MAX_BUF_SIZE / 4 - 1) {
	size = MAX_BUF_SIZE / 4 - 1;
	transmitPrioData(new RejectRetransmit(sequences[.. size - 1]));
	sequences = sequences[size ..];
    }

    if (size != 0) {
	transmitPrioData(new RejectRetransmit(sequences));
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
    transmitPrioData(new AckSequence(ackSeq));
}

/*
 * send a new TimeSynch every 20 seconds
 */
static void sync()
{
    call_out("sync", 20);
    bufData(new TimeSynch(new Time(0)), TRUE);
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
		    /* single fragment */
		    user->receiveMessage(fragment->body(), fragment->group());
		} else {
		    sequence = fragment->sequence();
		    blobs = fragmentQueue[sequence];
		    if (!blobs) {
			blobs = fragmentQueue[sequence] = ([ ]);
		    }
		    blobs[fragment->index()] = fragment->body();
		    if (map_sizeof(blobs) == count) {
			/* all fragments received */
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
	packet = new RemotePacket(str);
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
	    if (retransmitPacket(sequences[i])) {
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
	acknowledgedSeq(packet->data(PACKET_ACK_SEQUENCE)->sequence());
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
 * send a message
 */
void sendMessage(string message, int group, int required)
{
    if (previous_program() == OBJECT_PATH(User)) {
	call_out("bufMessage", 0, message, group, required);
    }
}


User user()	{ return user; }
int clientId()	{ return clientId; }
