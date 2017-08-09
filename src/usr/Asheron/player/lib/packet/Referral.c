# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int interfaceCookie;	/* time */
private int sessionCookie;
private int flags;		/* 2 */
private int port;		/* port to connect to */
private int address;		/* address to connect to */
private int serverId;		/* serverId */
private int something1;		/* 2002 */
private int something2;		/* something? */

/*
 * layout of Referral
 */
static string headerLayout()
{
    return "iissiiissi";
}

/*
 * size of the entire blob
 */
int size()
{
    return 32;
}

/*
 * export as a blob
 */
string transport()
{
    return serialize(headerLayout(), interfaceCookie, sessionCookie, flags,
		     port, address, 0, 0, serverId, something1, something2);
}

/*
 * create a Referral
 */
static void create(int interfaceCookie, int sessionCookie, int flags, int port,
		   int address, int serverId, int something1, int something2)
{
    ::create(PACKET_REFERRAL);

    ::interfaceCookie = interfaceCookie;
    ::sessionCookie = sessionCookie;
    ::flags = flags;
    ::port = port;
    ::address = address;
    ::serverId = serverId;
    ::something1 = something1;
    ::something2 = something2;
}

/*
 * fields
 */
int interfaceCookie()	{ return interfaceCookie; }
int sessionCookie()	{ return sessionCookie; }
int flags()		{ return flags; }
int port()		{ return port; }
int address()		{ return address; }
int serverId()		{ return serverId; }
int something1()	{ return something1; }
int something2()	{ return something2; }
