# include "Packet.h"

inherit Flow;


/*
 * create a Flow from a blob
 */
static void create(string blob)
{
    int prevSize, prevTime;

    ({
	blob,
	prevSize,
	prevTime,
    }) = deSerialize(blob, headerLayout());

    ::create(prevSize, prevTime);
}
