# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int time;		/* game time */
private float mtime;		/* game millitime */
private int interfaceCookie;	/* interface cookie (clone number) */
private int sessionCookie;	/* random session cookie */
private int clientId;		/* ID the client must use */
private int serverSeed;		/* ISAAC server seed */
private int clientSeed;		/* ISAAC client seed */
private int something;		/* flags?  PhatAC uses 0x2 here */

static string headerLayout()
{
    return "DDiiiiii";
}

int size()
{
    return 32;
}

string transport()
{
    return serialize(headerLayout(), time, mtime, interfaceCookie,
		     sessionCookie, clientId, serverSeed, clientSeed,
		     something);
}

static void create(int time, float mtime, int interfaceCookie,
		   int sessionCookie, int clientId, int serverSeed,
		   int clientSeed, int something)
{
    ::create(PACKET_CONNECT_REQUEST);

    ::time = time;
    ::mtime = mtime;
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
int time()		{ return time; }
float mtime()		{ return mtime; }
int interfaceCookie()	{ return interfaceCookie; }
int sessionCookie()	{ return sessionCookie; }
int clientId()		{ return clientId; }
int serverSeed()	{ return serverSeed; }
int clientSeed()	{ return clientSeed; }
int something()		{ return something; }