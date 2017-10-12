# include "Packet.h"

inherit RequestRetransmit;


/*
 * create a RequestTransmit from a blob
 */
static void create(string blob)
{
    int count;

    ({
	blob,
	count
    }) = deSerialize(blob, headerLayout());
    ::create(deSerialize(blob, "i", count)[1 ..]);
}
