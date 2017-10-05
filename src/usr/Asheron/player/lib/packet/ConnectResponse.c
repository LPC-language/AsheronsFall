# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int interfaceCookie;	/* interface cookie (clone index) */
private int sessionSeed;	/* random session seed */

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
    return serialize(headerLayout(), interfaceCookie, sessionSeed);
}

/*
 * create a ConnectResponse
 */
static void create(int interfaceCookie, int sessionSeed)
{
    ::create(PACKET_CONNECT_RESPONSE);

    ::interfaceCookie = interfaceCookie;
    ::sessionSeed = sessionSeed;
}

/*
 * fields
 */
int interfaceCookie()	{ return interfaceCookie; }
int sessionSeed()	{ return sessionSeed; }
