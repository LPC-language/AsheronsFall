# include "Packet.h"

inherit Flow;


/*
 * create a Flow from a blob
 */
static void create(string blob)
{
    int flowSize, flowTime;

    ({
	blob,
	flowSize,
	flowTime,
    }) = deSerialize(blob, headerLayout());

    ::create(flowSize, flowTime);
}
