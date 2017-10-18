# include "Packet.h"

inherit AckSequence;


/*
 * create an AckSequence from a blob
 */
static void create(string blob, int offset)
{
    int sequence;

    ({
	offset,
	sequence,
    }) = deSerialize(blob, offset, headerLayout());

    ::create(sequence);
}
