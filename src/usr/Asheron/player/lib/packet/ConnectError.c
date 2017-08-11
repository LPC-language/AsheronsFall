# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int unknown1;		/* ??? */
private int unknown2;		/* ??? */

/*
 * layout of ConnectError
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
    return serialize(headerLayout(), unknown1, unknown2);
}

/*
 * create a ConnectError
 */
static void create(int unknown1, int unknown2)
{
    ::create(PACKET_CONNECT_ERROR);

    ::unknown1 = unknown1;
    ::unknown2 = unknown2;
}

/*
 * fields
 */
int unknown1()	{ return unknown1; }
int unknown2()	{ return unknown2; }
