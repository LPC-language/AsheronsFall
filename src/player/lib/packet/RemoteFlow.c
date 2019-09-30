# include "Packet.h"

inherit Flow;


/*
 * create a Flow from a blob
 */
static void create(string blob, int offset)
{
    int flowSize, flowTime;

    ({
	offset,
	flowSize,
	flowTime,
    }) = deSerialize(blob, offset, headerLayout());

    ::create(flowSize, flowTime);
}
