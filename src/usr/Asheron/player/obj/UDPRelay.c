# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"
# include "RandSeq.h"

inherit Interface;


/*
 * AC uses different UDP ports to send and receive on.  This is an optimization
 * at the kernel level, where an internal lock on the output socket will
 * not block the input socket.
 */

# define MAX_PACKET_SIZE	484


Interface interface;		/* the interface to relay for */
int sessionTime;		/* session start time */
float sessionMTime;		/* session start millitime */
int serverId;			/* server ID */
RandSeq serverRand;		/* for checksum encryption */
int pendingTransmit;		/* transmit callout active? */
NetworkData *priorityQueue;	/* priority data */
mixed *transmitQueue;		/* packets to be transmitted */
mapping transmitBuffer;		/* packets that may have to be retransmitted */
int serverSeq;			/* highest assigned seq */
int ackSeq;			/* highest seq acknowledged by the client */

/*
 * initialize
 */
static void create()
{
    priorityQueue = ({ });
    transmitQueue = ({ });
    transmitBuffer = ([ ]);
    serverSeq = ackSeq = 1;
}

/*
 * transmit one packet per task to the client
 */
static void transmit()
{
    NetworkData data;
    Packet packet;
    int time, flags;
    float mtime, session;

    if (sizeof(priorityQueue) != 0) {
	data = priorityQueue[0];
	priorityQueue = priorityQueue[1 ..];

	if (data <- Packet) {
	    /* retransmission */
	    packet = data;
	    data = nil;
	} else if (sizeof(transmitQueue) != 0) {
	    packet = transmitQueue[0];
	    if (packet->size() + data->size() <= MAX_PACKET_SIZE) {
		transmitQueue = transmitQueue[1 ..];
	    } else {
		packet = new Packet(0, serverId, 0);
	    }
	} else {
	    packet = new Packet(0, serverId, 0);
	}
    } else if (sizeof(transmitQueue) != 0) {
	packet = transmitQueue[0];
	transmitQueue = transmitQueue[1 ..];
    } else {
	pendingTransmit = 0;
	return;
    }

    ({ time, mtime }) = millitime();
    session = timeDiff(sessionTime, sessionMTime, time, mtime);

    flags = packet->flags();
    if (!(flags & PACKET_RETRANSMISSION)) {
	if (flags & PACKET_ENCRYPTED_CHECKSUM) {
	    packet->setSequence(++serverSeq);
	    packet->setXorValue(serverRand->rand(serverSeq - 2));
	} else {
	    packet->setSequence(serverSeq);
	}
	if (flags & PACKET_REQUIRED) {
	    transmitBuffer[serverSeq] = packet;
	}
    }
    if (flags & PACKET_TIME_SYNCH) {
	packet->addData(new TimeSynch(timeServer(time, mtime)...));
    }
    if (flags & PACKET_ECHO_RESPONSE) {
	float clientTime;

	clientTime = packet->data(PACKET_ECHO_RESPONSE)->clientTime();
	packet->addData(new EchoResponse(session - clientTime, clientTime));
    }
    if (data) {
	if (flags & PACKET_REQUIRED) {
	    packet = packet->duplicate();
	}
	packet->addData(data);
    }
    packet->setTime((int) floor(ldexp(session, 1)));
    message(packet->transport());

    pendingTransmit = call_out("transmit", 0.005); /* XXX flow-based timing */
}

/*
 * add a packet to the transmit queue
 */
static void _transmitPacket(Packet packet)
{
    packet->setEncryptedChecksum();
    transmitQueue += ({ packet });
    if (pendingTransmit == 0) {
	transmit();
    }
}

/*
 * callout wrapper for _transmitPacket
 */
void transmitPacket(Packet packet)
{
    if (previous_program() == OBJECT_PATH(UDPInterface)) {
	call_out("_transmitPacket", 0, packet);
    }
}

/*
 * add data to the priority queue
 */
static void _transmitPrioData(NetworkData data)
{
    priorityQueue += ({ data });
    if (pendingTransmit == 0) {
	transmit();
    }
}

/*
 * callout wrapper for _transmitPrioData
 */
void transmitPrioData(NetworkData data)
{
    if (previous_program() == OBJECT_PATH(UDPInterface)) {
	call_out("_transmitPrioData", 0, data);
    }
}

/*
 * retransmit a packet that was requested by the client
 */
int retransmitPacket(int sequence)
{
    if (previous_object() == interface && sequence <= serverSeq) {
	Packet packet;

	packet = transmitBuffer[sequence];
	if (packet) {
	    packet->setRetransmission();
	    _transmitPrioData(packet);
	    return TRUE;
	}
    }
    return FALSE;
}

/*
 * a sequence was acknowledged by the client
 */
static void _acknowledgedSeq(int sequence)
{
    if (sequence <= serverSeq) {
	while (ackSeq < sequence) {
	    transmitBuffer[++ackSeq] = nil;
	}
    }
}

/*
 * callout wrapper for _acknowledgedSeq
 */
void acknowledgedSeq(int sequence)
{
    if (previous_program() == OBJECT_PATH(UDPInterface)) {
	call_out("_acknowledgedSeq", 0, sequence);
    }
}


/*
 * start a relay connection
 */
static int _login(string str, object connObj)
{
    Packet packet;
    ConnectResponse connectResponse;
    int clientId, interfaceCookie, serverSeed;

    catch {
	packet = new ClientPacket(str);
    } : {
	/* drop connection without sending anything back */
	return MODE_DISCONNECT;
    }
    if (packet->checksum() != packet->computeChecksum()) {
	/* corrupted packet */
	return MODE_DISCONNECT;
    }

    if (packet->flags() != PACKET_CONNECT_RESPONSE) {
	/* don't bother responding to anything but a ConnectResponse */
	return MODE_DISCONNECT;
    }

    connection(connObj);
    ({ sessionTime, sessionMTime }) = millitime();

    /* verify that all parameters are correct */
    clientId = packet->id();
    connectResponse = packet->data(PACKET_CONNECT_RESPONSE);
    interfaceCookie = connectResponse->interfaceCookie();
    serverSeed = connectResponse->sessionSeed();
    serverRand = new RandSeq(serverSeed);
    interface = find_object(OBJECT_PATH(UDPInterface) + "#" + interfaceCookie);
    if (!interface) {
	return MODE_DISCONNECT;
    }
    serverId = interface->serverId();
    if (!interface->establish(this_object(), clientId, serverSeed)) {
	return MODE_DISCONNECT;
    }

    return MODE_NOCHANGE;
}

/*
 * login with limits
 */
int login(string str)
{
    if (previous_program() == LIB_CONN) {
	return call_limited("_login", str, previous_object());
    }
    return MODE_NOCHANGE;
}

/*
 * destruct interface on logout
 */
void logout(int quit)
{
    if (previous_program() == LIB_CONN) {
	destruct_object(this_object());
    }
}

/*
 * ignore all input here; the client should send directly to the interface
 */
int receive_message(string str)
{
    return MODE_NOCHANGE;
}

/*
 * let the main interface terminate the connection
 */
void disconnect()
{
    if (previous_object() == interface) {
	::disconnect();
    }
}


object interface()	{ return interface; }
