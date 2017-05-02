# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"

inherit Interface;


Interface interface;

int login(string str)
{
    if (previous_program() == LIB_CONN) {
	Packet packet;
	ConnectResponse connectResponse;
	int interfaceCookie, sessionCookie;

	catch {
	    packet = new ClientPacket(str);
	} : {
	    return MODE_DISCONNECT;
	}

	if (packet->flags() != PACKET_CONNECT_RESPONSE) {
	    return MODE_DISCONNECT;
	}
	connectResponse = packet->getData(PACKET_CONNECT_RESPONSE);
	interfaceCookie = connectResponse->interfaceCookie();
	sessionCookie = connectResponse->sessionCookie();

	interface = find_object(OBJECT_PATH(UDPInterface) + "#" +
				interfaceCookie);
	if (!interface || !interface->establish(this_object(), sessionCookie)) {
	    return MODE_DISCONNECT;
	}

	connection(previous_object());
    }
    return MODE_NOCHANGE;
}

void logout(int quit)
{
    if (previous_program() == LIB_CONN) {
	destruct_object(this_object());
    }
}

int receive_message(string str)
{
    /*
     * ignore all input here; the client should send directly to the interface
     */
    return MODE_NOCHANGE;
}

int relaySend(string str)
{
    if (previous_object() == interface) {
	return message(str);
    }
}

void relayQuit()
{
    if (previous_object() == interface) {
	disconnect();
    }
}
