# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private int something1;		/* ??? */
private int something2;		/* ??? */

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
    return serialize(headerLayout(), something1, something2);
}

/*
 * create a ConnectError
 */
static void create(int something1, int something2)
{
    ::create(PACKET_CONNECT_ERROR);

    ::something1 = something1;
    ::something2 = something2;
}

/*
 * fields
 */
int something1()	{ return something1; }
int something2()	{ return something2; }
