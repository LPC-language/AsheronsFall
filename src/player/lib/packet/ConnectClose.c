# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private int something1;		/* ??? */
private int something2;		/* ??? */

/*
 * layout of ConnectClose
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
    return serialize(headerLayout(), something1, something2);
}

/*
 * create a ConnectClose
 */
static void create(int unknown1, int unknown2)
{
    ::create(PACKET_CONNECT_CLOSE);

    ::something1 = something1;
    ::something2 = something2;
}

/*
 * fields
 */
int something1()	{ return something1; }
int something2()	{ return something2; }
