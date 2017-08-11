# include "Packet.h"

inherit ConnectError;


/*
 * create a ConnectError from a blob
 */
static void create(string blob)
{
    int unknown1, unknown2;

    ({
	blob,
	unknown1,
	unknown2,
    }) = deSerialize(blob, headerLayout());

    ::create(unknown1, unknown2);
}
