# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int interfaceCookie;	/* interface cookie (clone index) */
private int sessionCookie;	/* random session cookie */

/*
 * layout of ConnectResponse
 */
static string headerLayout()
{
    return "ii";
}

/*
 * size of entire blob
 */
int size()
{
    return 8;
}

/*
 * export as a blob
 */
string transport()
{
    return serialize(headerLayout(), interfaceCookie, sessionCookie);
}

/*
 * create a ConnectResponse
 */
static void create(int interfaceCookie, int sessionCookie)
{
    ::create(PACKET_CONNECT_RESPONSE);

    ::interfaceCookie = interfaceCookie;
    ::sessionCookie = sessionCookie;
}

/*
 * fields
 */
int interfaceCookie()	{ return interfaceCookie; }
int sessionCookie()	{ return sessionCookie; }
