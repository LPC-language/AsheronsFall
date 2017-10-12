# include "Packet.h"

inherit ServerSwitch;


/*
 * create a ServerSwitch from a blob
 */
static void create(string blob)
{
    int serverId, something;

    ({
	blob,
	serverId,
	something
    }) = deSerialize(blob, headerLayout());
    ::create(serverId, something);
}
