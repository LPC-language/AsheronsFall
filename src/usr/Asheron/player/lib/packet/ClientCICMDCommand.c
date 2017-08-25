# include "Packet.h"

inherit CICMDCommand;


/*
 * create a CICMDCommand from a blob
 */
static void create(string blob)
{
    int command;
    int parameter;

    ({
	blob,
	command,
	parameter
    }) = deSerialize(blob, headerLayout());
    ::create(command, parameter);
}
