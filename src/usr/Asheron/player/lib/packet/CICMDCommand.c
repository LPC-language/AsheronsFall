# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int command;		/* unknown */
private int param;		/* unknown */

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
    return serialize(headerLayout(), command, param);
}

/*
 * create a CICMDCommand
 */
static void create(int command, int param)
{
    ::create(PACKET_CICMD_COMMAND);

    ::command = command;
    ::param = param;
}

/*
 * fields
 */
int command()	{ return command; }
int param()	{ return param; }
