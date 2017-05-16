# include "Packet.h"

inherit AckSequence;


/*
 * create an AckSequence from a blob
 */
static void create(string blob)
{
    int sequence;

    ({
	blob,
	sequence,
    }) = deSerialize(blob, headerLayout());

    ::create(sequence);
}
