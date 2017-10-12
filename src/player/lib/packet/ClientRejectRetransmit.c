# include "Packet.h"

inherit RejectRetransmit;


/*
 * create a RejectTransmit from a blob
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
