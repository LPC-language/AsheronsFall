# include "Packet.h"

inherit CICMDCommand;


/*
 * create a CICMDCommand from a blob
 */
static void create(string blob, int offset)
{
    int command;
    int parameter;

    ({
	offset,
	command,
	parameter
    }) = deSerialize(blob, offset, headerLayout());
    ::create(command, parameter);
}
