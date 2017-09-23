# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"

inherit PacketInterface;


# define CLIENT_VERSION	"1802"
# define SERVER_ID	0xAC

string loginBlob;	/* first packet blob received */
int sessionCookie;	/* random cookie which the client has to echo */
Interface relay;	/* outgoing packet relay */

/*
 * keep sending ConnectionRequests until client responds, or timeout
 */
static void sendConnectRequest(string connectBlob, int tries)
{
    if (!relay) {
	if (tries == 0) {
	    disconnect();
	} else {
	    ::message(connectBlob);
	    call_out("sendConnectRequest", 1, connectBlob, tries - 1);
	}
    }
}

/*
 * start a UDP connection
 */
static int _login(string str, object connObj)
{
    Packet packet;
    LoginRequest loginRequest;
    string name, password;
    int interfaceCookie, clientId, serverSeed, clientSeed, time;
    float mtime;

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

    if (packet->flags() != PACKET_LOGIN_REQUEST) {
	/* don't bother responding to anything but a login request */
	return MODE_DISCONNECT;
    }

    loginBlob = str;
    loginRequest = packet->data(PACKET_LOGIN_REQUEST);
    if (loginRequest->version() != CLIENT_VERSION) {
	/* ignore login attempts from older clients */
	return MODE_DISCONNECT;
    }
    name = loginRequest->account();
    password = loginRequest->ticket();
    if (password) {
	sscanf(password, "\n%s", password);
    } else {
	/* PhatAC method */
	sscanf(name, "%s:%s", name, password);
    }
    if (!password) {
	/* missing password */
	return MODE_DISCONNECT;
    }

    connection(connObj);
    sscanf(object_name(this_object()), "%*s#%d", interfaceCookie);
    clientId = interfaceCookie & 0xffff;
    sessionCookie = random(0) & 0xffffffff;
    serverSeed = random(0) & 0xffffffff;
    clientSeed = random(0) & 0xffffffff;
    if (!::login(name, password, SERVER_ID, clientId, serverSeed, clientSeed)) {
	/* login failed */
	return MODE_DISCONNECT;
    }

    ({ time, mtime }) = gameTime();
    packet = new Packet(0, 0, SERVER_ID, 0, 0);
    packet->addData(new ConnectRequest(time, mtime, interfaceCookie,
				       sessionCookie, clientId, serverSeed,
				       clientSeed, 0));
    sendConnectRequest(packet->transport(), 5);
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
 * also disconnect the relay on logout
 */
void logout(int quit)
{
    if (previous_program() == LIB_CONN) {
	::logout();
	if (relay) {
	    relay->disconnect();
	}
	destruct_object(this_object());
    }
}

/*
 * establish a relay for this interface
 */
int establish(Interface relay, int clientId, int cookie)
{
    if (previous_program() == OBJECT_PATH(UDPRelay) && !::relay &&
	clientId == clientId() && cookie == sessionCookie) {
	::relay = relay;
	::establish();
	return TRUE;
    }
    return FALSE;
}

/*
 * transmit a packet through the relay
 */
static void transmit(Packet packet, int required)
{
    relay->transmit(packet, required);
}

/*
 * ask the relay to retransmit a packet
 */
static int retransmit(int sequence)
{
    return relay->retransmit(sequence);
}

/*
 * tell the relay that the client acknowledged packets
 */
static void acknowledged(int sequence)
{
    relay->acknowledged(sequence);
}

/*
 * pass on an incoming packet to the lower level
 */
int receive_message(string str)
{
    if (previous_program() == LIB_CONN && str != loginBlob) {
	return call_limited("receivePacket", str);
    }
    return MODE_NOCHANGE;
}


object relay() { return relay; }
