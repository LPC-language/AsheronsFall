# include "Packet.h"

inherit RequestRetransmit;


/*
 * create a RequestTransmit from a blob
 */
static void create(string blob, int offset)
{
    int count;

    ({
	offset,
	count
    }) = deSerialize(blob, offset, headerLayout());
    ::create(deSerialize(blob, offset, "i", count)[1 ..]);
}
