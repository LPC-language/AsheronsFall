# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private float time;		/* game time */
private int interfaceCookie;	/* interface cookie (clone number) */
private int sessionCookie;	/* random session cookie */
private int clientId;		/* ID the client must use */
private int serverSeed;		/* ISAAC server seed */
private int clientSeed;		/* ISAAC client seed */
private int something;		/* flags?  PhatAC uses 0x2 here */

static string headerLayout()
{
    return "diiiiii";
}

int size()
{
    return 32;
}

string transport()
{
    return serialize(headerLayout(), time, interfaceCookie, sessionCookie,
		     clientId, serverSeed, clientSeed, something);
}

static void create(float time, int interfaceCookie, int sessionCookie,
		   int clientId, int serverSeed, int clientSeed, int something)
{
    ::create(PACKET_CONNECT_REQUEST);

    ::time = time;
    ::interfaceCookie = interfaceCookie;
    ::sessionCookie = sessionCookie;
    ::clientId = clientId;
    ::serverSeed = serverSeed;
    ::clientSeed = clientSeed;
    ::something = something;
}

/*
 * fields
 */
float time()		{ return time; }
int interfaceCookie()	{ return interfaceCookie; }
int sessionCookie()	{ return sessionCookie; }
int clientId()		{ return clientId; }
int serverSeed()	{ return serverSeed; }
int clientSeed()	{ return clientSeed; }
int something()		{ return something; }
