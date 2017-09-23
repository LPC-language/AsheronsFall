# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"

inherit Interface;


/*
 * AC uses different UDP ports to send and receive on.  This is an optimization
 * at the kernel level, where an internal lock on the output socket will
 * not block the input socket.
 */

Interface interface;		/* the interface to relay for */
Packet *transmitQueue;		/* packets to be transmitted */
mapping transmitBuffer;		/* packets that may have to be retransmitted */
int transmitSeq;		/* highest transmitted seq */
int acknowledgedSeq;		/* highest seq acknowledged by the client */

/*
 * initialize
 */
static void create()
{
    transmitQueue = ({ });
    transmitBuffer = ([ ]);
}

/*
 * transmit one packet per task to the client
 */
static void transmitPacket()
{
    transmitQueue = transmitQueue[1 ..];

    if (sizeof(transmitQueue) != 0) {
	Packet packet;
	int sequence;

	packet = transmitQueue[0];
	sequence = packet->sequence();
	if (sequence > transmitSeq) {
	    transmitSeq = sequence;
	}
	message(packet->transport());

	call_out("transmitPacket", 0.005);	/* XXX flow-based timing */
    }
}

/*
 * add a packet to the transmit queue
 */
void transmit(Packet packet, int required)
{
    if (previous_object() == interface) {
	if (sizeof(transmitQueue) == 0) {
	    transmitQueue = ({ nil, packet });
	    transmitPacket();
	} else {
	    transmitQueue += ({ packet });
	}
	if (required) {
	    transmitBuffer[packet->sequence()] = packet;
	}
    }
}

/*
 * retransmit a packet that was requested by the client
 */
int retransmit(int sequence)
{
    if (previous_object() == interface && sequence <= transmitSeq) {
	Packet packet;

	packet = transmitBuffer[sequence];
	if (packet) {
	    packet->addRetransmission();
	    transmit(packet, FALSE);
	    return TRUE;
	}
    }
    return FALSE;
}

/*
 * a sequence was acknowledged by the client
 */
void acknowledged(int sequence)
{
    if (previous_object() == interface && sequence <= transmitSeq) {
	while (acknowledgedSeq < sequence) {
	    transmitBuffer[++acknowledgedSeq] = nil;
	}
    }
}


/*
 * start a relay connection
 */
static int _login(string str, object connObj)
{
    Packet packet;
    ConnectResponse connectResponse;
    int clientId, interfaceCookie, sessionCookie;

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

    /* verify that all parameters are correct */
    clientId = packet->id();
    connectResponse = packet->data(PACKET_CONNECT_RESPONSE);
    interfaceCookie = connectResponse->interfaceCookie();
    sessionCookie = connectResponse->sessionCookie();
    interface = find_object(OBJECT_PATH(UDPInterface) + "#" + interfaceCookie);
    if (!interface ||
	!interface->establish(this_object(), clientId, sessionCookie)) {
	return MODE_DISCONNECT;
    }

    connection(connObj);
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
