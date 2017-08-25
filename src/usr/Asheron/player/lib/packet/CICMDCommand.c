# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int command;		/* unknown */
private int parameter;		/* unknown */

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
    return serialize(headerLayout(), command, parameter);
}

/*
 * create a CICMDCommand
 */
static void create(int command, int parameter)
{
    ::create(PACKET_CICMD_COMMAND);

    ::command = command;
    ::parameter = parameter;
}

/*
 * fields
 */
int command()	{ return command; }
int parameter()	{ return parameter; }
