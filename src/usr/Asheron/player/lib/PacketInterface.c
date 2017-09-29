# include <kernel/user.h>
# include <status.h>
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

static void transmit(Packet packet, int time, int required);
static int retransmit(int sequence, int time);
static void acknowledged(int sequence);


/*
 * timing
 */

private int startTime;		/* start of this session */
private int time;		/* saved time */
private float mtime;		/* saved mtime */

/*
 * initialize time handling
 */
private void timeCreate()
{
    ({ time, mtime }) = millitime();
    startTime = time;
}

/*
 * save the current time
 */
private void timeSave()
{
    ({ time, mtime }) = millitime();
}

/*
 * time since the server first booted
 */
static mixed *timeServer()
{
    return ({ time - status(ST_STARTTIME), mtime });
}

/*
 * the packet time field
 */
private int timePacket()
{
    return (time - startTime) * 2 + (int) ((mtime + 0.05) * 2.0);
}


/*
 * packet output buffer
 */

# define MAX_BUF_SIZE		464
# define FRAG_HEADER_SIZE	16
# define MAX_FRAG_SIZE		(MAX_BUF_SIZE - FRAG_HEADER_SIZE)
# define FRAG_ID		0x03000000

private int serverId;		/* ID of the server */
private RandSeq serverRand;	/* for checksum encryption */
private Packet bufPacket;	/* buffered partial packet */
private int bufSize;		/* packet buffer space used */
private int bufRequired;	/* is current buffered packet required? */
private int drain;		/* buffer drain callout active? */
private int serverSeq;		/* seq of last packet sent */
private int serverFrag;		/* seq of last fragment sent */
private int timeSync;		/* timeSync needed? */

/*
 * create packet output buffer
 */
private void bufCreate(int serverId, RandSeq serverRand)
{
    ::serverId = serverId;
    ::serverRand = serverRand;
    serverSeq = 1;
}

/*
 * start a new packet output buffer
 */
private void bufStart()
{
    bufPacket = new Packet(serverSeq + 1, 0, serverId, 0);
    bufRequired = timeSync = FALSE;
    if (!drain) {
	drain = TRUE;
	call_out("bufDrain", 0.005, bufPacket);
    }
}

/*
 * flush the packet output buffer
 */
private void bufFlush()
{
    if (bufPacket) {
	if (timeSync) {
	    bufPacket->addData(new TimeSynch(timeServer()...));
	}
	bufPacket->setEncryptedChecksum();
	bufPacket->setXorValue(serverRand->rand(serverSeq - 1));
	transmit(bufPacket, timePacket(), bufRequired);
	++serverSeq;
	bufPacket = nil;
	bufSize = 0;
    }
}

/*
 * drain the packet output buffer after 5 millisecs
 */
static void bufDrain(Packet packet)
{
    int drainTime;
    float drainMtime;

    if (packet == bufPacket) {
	bufFlush();
    } else if (bufPacket) {
	({ drainTime, drainMtime }) = millitime();
	drainMtime -= mtime;
	if (drainMtime < 0.0) {
	    drainMtime++;
	    --drainTime;
	}
	drainTime -= time;
	if (drainTime > 0 || drainMtime >= 0.005) {
	    bufFlush();
	} else {
	    drain = TRUE;
	    call_out("bufDrain", 0.005 - drainMtime, bufPacket);
	    return;
	}
    }

    drain = FALSE;
}

/*
 * add optional header to output packet
 */
private void bufData(NetworkData data, int required)
{
    int size;

    if (bufSize != 0 && !(bufPacket->flags() & data->networkDataType())) {
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
 * add TimeSync to output packet
 */
private void bufTimeSync()
{
    if (!bufPacket) {
	bufStart();
    }
    timeSync = TRUE;
    bufRequired = TRUE;

    bufSize += 8;
    if (bufSize == MAX_BUF_SIZE) {
	bufFlush();
    }
}

/*
 * add flow to output packet
 */
private void bufFlow(int flowSize, int flowTime)
{
    if (bufPacket &&
	(bufSize > MAX_BUF_SIZE - 6 || (bufPacket->flags() & PACKET_FLOW))) {
	bufFlush();
    }
    if (!bufPacket) {
	bufStart();
    }
    bufPacket->addData(new Flow(flowSize, flowTime));
    bufRequired = TRUE;

    bufSize += 6;
    if (bufSize == MAX_BUF_SIZE) {
	bufFlush();
    }
}

/*
 * send a message to the client
 */
void sendMessage(string message, int group, int required)
{
    timeSave();
    bufMessage(message, group, required);
}

/*
 * request the client to retransmit some packets
 */
private void requestRetransmit(int *sequences)
{
    int size;
    Packet packet;

    while (bufSize > MAX_BUF_SIZE - 4 - (size=sizeof(sequences)) * 4) {
	/* bypass the packet buffer */
	packet = new Packet(serverSeq, 0, serverId, 0);
	if (size > MAX_BUF_SIZE / 4 - 1) {
	    size = MAX_BUF_SIZE / 4 - 1;
	}
	packet->addData(new RequestRetransmit(sequences[.. size - 1]));
	transmit(packet, timePacket(), FALSE);
	sequences = sequences[size ..];
    }

    if (size != 0) {
	if (!bufPacket) {
	    bufStart();
	}
	bufData(new RequestRetransmit(sequences), FALSE);
	bufFlush();
    }
}

/*
 * reject a retransmission request from the client
 */
private void rejectRetransmit(int *sequences)
{
    int size;
    Packet packet;

    while (bufSize > MAX_BUF_SIZE - 4 - (size=sizeof(sequences)) * 4) {
	/* bypass the packet buffer */
	packet = new Packet(serverSeq, 0, serverId, 0);
	if (size > MAX_BUF_SIZE / 4 - 1) {
	    size = MAX_BUF_SIZE / 4 - 1;
	}
	packet->addData(new RejectRetransmit(sequences[.. size - 1]));
	transmit(packet, timePacket(), FALSE);
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
 * acknowledge packets received from the client
 */
private void ackSequence(int sequence)
{
    Packet packet;

    if (bufSize > MAX_BUF_SIZE - 4) {
	/* bypass the packet buffer */
	packet = new Packet(serverSeq, 0, serverId, 0);
	packet->addData(new AckSequence(sequence));
	transmit(packet, timePacket(), FALSE);
    } else {
	if (!bufPacket) {
	    bufStart();
	}
	bufData(new AckSequence(sequence), FALSE);
	/* XXX bufFlush() needed? */
    }
}


/*
 * input message handling
 */

private int clientId;		/* session ID */
private RandSeq clientRand;	/* for checksum encryption */
private int receiveSeq;		/* highest seq received */
private int clientSeq;		/* seq last processed */
private int clientAck;		/* seq acked by server */
private mapping clientQueue;
private mapping fragmentQueue;

private void clientCreate(int clientId, RandSeq clientRand)
{
    ::clientId = clientId;
    ::clientRand = clientRand;
    clientSeq = 1;
    clientQueue = ([ ]);
    fragmentQueue = ([ ]);
}

static void clientPacket(Packet packet, int sequence)
{
    Packet nextPacket;
    int flags;

    clientSeq = sequence;

    if ((nextPacket=clientQueue[++sequence])) {
	clientQueue[sequence] = nil;
	call_out("clientPacket", 0, nextPacket, sequence);
    }

    /* XXX process options */
    flags = packet->flags();
    /* timeSynch */
    /* flow */
    /* echo request */

    /* XXX process fragments */

    /*
    account->receiveMessage(message, group);
    */
}


private User user;		/* connected account */
private string loginError;

/*
 * login during 3-way handshake
 */
static int login(string name, string password, int serverId, int clientId,
		 int serverSeed, int clientSeed)
{
    string message;

    timeCreate();
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

    bufCreate(serverId, new RandSeq(serverSeed));
    clientCreate(clientId, new RandSeq(clientSeed));

    return TRUE;
}

/*
 * connection was established
 */
static void establish()
{
    if (loginError) {
	timeSave();
	bufMessage(loginError, 9, TRUE);
	bufFlush();
    } else {
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
    int sequence, flags;

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
	    return MODE_NOCHANGE;	/* duplicate */
	}
	packet->setXorValue(clientRand->rand(sequence - 2));
    } else if (flags & ~(PACKET_REQUEST_RETRANSMIT | PACKET_REJECT_RETRANSMIT |
			 PACKET_ACK_SEQUENCE | PACKET_DISCONNECT |
			 PACKET_CONNECT_ERROR | PACKET_CICMD_COMMAND)) {
	return MODE_NOCHANGE;		/* missing encrypted checksum */
    } else if (sequence == 0) {
	if (flags & ~(PACKET_DISCONNECT | PACKET_CICMD_COMMAND)) {
	    return MODE_NOCHANGE;
	}
    } else if (sequence <= clientSeq - 10) {
	return MODE_NOCHANGE;		/* too far back */
    }
    if (packet->checksum() != packet->computeChecksum()) {
	return MODE_NOCHANGE;		/* corrupted or different session */
    }

    switch (flags) {
    case PACKET_DISCONNECT:
	return MODE_DISCONNECT;

    case 0:
    case PACKET_ENCRYPTED_CHECKSUM:
    case PACKET_CONNECT_ERROR:
    case PACKET_CICMD_COMMAND:
	return MODE_NOCHANGE;
    }

    timeSave();

    /*
     * handle urgent requests out of order
     */
    if (flags & PACKET_REQUEST_RETRANSMIT) {
	int sz, i, *sequences;

	sequences = packet->requestRetransmit()->sequences();
	for (sz = sizeof(sequences), i = 0; i < sz; i++) {
	    if (retransmit(sequences[i], timePacket())) {
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

	sequences = packet->rejectRetransmit()->sequences();
	for (sz = sizeof(sequences), i = 0; i < sz; i++) {
	    clientSequence = sequences[i];
	    if (clientSequence > clientSeq && clientSequence <= receiveSeq &&
		!clientQueue[clientSequence]) {
		/* inject fake packet to fill the gap */
		clientQueue[clientSequence] =
				    new Packet(clientSequence, 0, clientId, 0);
	    }
	}
    }
    if (flags & PACKET_ACK_SEQUENCE) {
	acknowledged(packet->data(PACKET_ACK_SEQUENCE)->sequence());
    }

    if (receiveSeq < sequence) {
	receiveSeq = sequence;
    }
    if (sequence == clientSeq + 1) {
	clientPacket(packet, sequence);
    } else {
	clientQueue[sequence] = packet;
	/* XXX request retransmission, after a while */
    }

    return MODE_NOCHANGE;
}


User user()	{ return user; }
int clientId()	{ return clientId; }
