# include "Packet.h"

inherit ServerSwitch;


/*
 * create a ServerSwitch from a blob
 */
static void create(string blob, int offset)
{
    int serverId, something;

    ({
	offset,
	serverId,
	something
    }) = deSerialize(blob, offset, headerLayout());
    ::create(serverId, something);
}
