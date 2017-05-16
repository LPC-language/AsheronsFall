# include "Packet.h"

inherit TimeSynch;


/*
 * create a TimeSynch from a blob
 */
static void create(string blob)
{
    float time;

    ({
	blob,
	time
    }) = deSerialize(blob, headerLayout());
    ::create(time);
}
