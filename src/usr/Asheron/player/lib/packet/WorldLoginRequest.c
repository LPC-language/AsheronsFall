# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int interfaceCookie;	/* interface cookie */
private int sessionCookie;	/* session cookie */

/*
 * layout of WorldLoginRequest
 */
static string headerLayout()
{
    return "ii";
}

/*
 * size of the entire blob
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
 * create a WorldLoginRequest
 */
static void create(int interfaceCookie, int sessionCookie)
{
    ::interfaceCookie = interfaceCookie;
    ::sessionCookie = sessionCookie;
}


int interfaceCookie()	{ return interfaceCookie; }
int sessionCookie()	{ return sessionCookie; }
