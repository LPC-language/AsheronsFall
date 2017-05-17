# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int something1;		/* unknown */
private int something2;		/* unknown */

/*
 * assumed layout of CICMDCommand
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
 * create a CICMDCommand
 */
static void create(int something1, int something2)
{
    ::create(PACKET_CICMD_COMMAND);

    ::something1 = something1;
    ::something2 = something2;
}

/*
 * fields
 */
int something1()	{ return something1; }
int something2()	{ return something2; }
