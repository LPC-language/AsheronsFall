# include <kernel/user.h>
# include "Packet.h"
# include "Interface.h"

inherit PacketInterface;


# define CLIENT_VERSION	"1802"
# define SERVER_ID	0xAC

string loginBlob;	/* first packet blob received */
int serverSeed;		/* server seed which the client has to echo */
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
    int interfaceCookie, clientId, clientSeed;

    catch {
	packet = new RemotePacket(str);
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
    serverSeed = random(0) & 0xffffffff;
    clientSeed = random(0) & 0xffffffff;
    if (!::login(name, password, SERVER_ID, clientId, clientSeed)) {
	/* login failed */
	return MODE_DISCONNECT;
    }

    packet = new Packet(0, SERVER_ID, 0);
    packet->addData(new ConnectRequest(timeServer(millitime()...),
				       interfaceCookie, serverSeed, clientId,
				       serverSeed, clientSeed, 0));
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
int establish(Interface relay, int clientId, int seed)
{
    if (previous_program() == OBJECT_PATH(UDPRelay) && !::relay &&
	clientId == clientId() && seed == serverSeed) {
	::relay = relay;
	::establish();
	return TRUE;
    }
    return FALSE;
}

/*
 * transmit packets through the relay
 */
static void transmitPackets(Packet *packets)
{
    relay->transmitPackets(packets);
}

/*
 * transmit priority data through the relay
 */
static void transmitPrioData(NetworkData data)
{
    relay->transmitPrioData(data);
}

/*
 * ask the relay to retransmit a packet
 */
static int retransmitPacket(int sequence)
{
    return relay->retransmitPacket(sequence);
}

/*
 * tell the relay that the client acknowledged packets
 */
static void acknowledgedSeq(int sequence)
{
    relay->acknowledgedSeq(sequence);
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


int serverId()	{ return SERVER_ID; }
object relay()	{ return relay; }
