# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"
# include "Account.h"

inherit PacketInterface;


# define LEAFCULL_BIRTH		862952622
# define SERVER_ID		0xAC

string name;		/* account name */
int clientId;		/* ID in messages from the client */
int sessionCookie;	/* random cookie which the client has to echo */
int serverSeed;		/* ISAAC seed for server */
int clientSeed;		/* ISAAC seed for client */
Interface relay;	/* outgoing packet relay */

private float gameTime()
{
    int time;
    float milli;

    ({ time, milli }) = millitime();
    return (float) (time - LEAFCULL_BIRTH) + milli;
}

static void sendConnectRequest(string connectBlob, int tries)
{
    if (!relay) {
	if (tries == 0) {
	    disconnect();
	} else {
	    message(connectBlob);
	    call_out("sendConnectRequest", 1, connectBlob, tries - 1);
	}
    }
}

int login(string str)
{
    if (previous_program() == LIB_CONN) {
	Packet packet;
	LoginRequest loginRequest;
	string ticket, password;
	int interfaceCookie;

	catch {
	    packet = new ClientPacket(str);
	} : {
	    /* drop connection without sending anything back */
	    return MODE_DISCONNECT;
	}

	connection(previous_object());

	if (packet->flags() != PACKET_LOGIN_REQUEST) {
	    /* XXX send back some error */
	    return MODE_DISCONNECT;
	}
	loginRequest = packet->getData(PACKET_LOGIN_REQUEST);
	name = loginRequest->account();
	ticket = loginRequest->ticket();
	if (ticket) {
	    sscanf(ticket, "\n%s", password);
	} else {
	    /* PhatAC method */
	    sscanf(name, "%s:%s", name, password);
	}
	/* XXX account/password check XXX */

	sscanf(object_name(this_object()), "%*s#%d", interfaceCookie);
	clientId = interfaceCookie & 0xffff;
	sessionCookie = random(0) & 0xffffffff;
	serverSeed = random(0) & 0xffffffff;
	clientSeed = random(0) & 0xffffffff;
	packet = new Packet(0, 0, 0, SERVER_ID, 0, 0);
	packet->addData(new ConnectRequest(gameTime(), interfaceCookie,
					   sessionCookie, clientId,
					   serverSeed, clientSeed, 0));
	sendConnectRequest(packet->transport(), 5);
    }
    return MODE_NOCHANGE;
}

void logout(int quit)
{
    if (previous_program() == LIB_CONN) {
	::logout();
	destruct_object(this_object());
    }
}

int n;

int receive_message(string str)
{
    if (previous_program() == LIB_CONN) {
	write_file("/usr/Asheron/data/r" + n++, str);
    }
    return MODE_NOCHANGE;
}

int establish(Interface relay, int cookie)
{
    if (previous_program() == OBJECT_PATH(UDPRelay) && !::relay &&
	cookie == sessionCookie) {
	::relay = relay;
	::login(name);
	return TRUE;
    }
    return FALSE;
}
