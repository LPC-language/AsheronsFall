# include "Packet.h"

inherit TimeSynch;


/*
 * create a TimeSynch from a blob
 */
static void create(string blob)
{
    int time;
    float mtime;

    ({
	blob,
	time,
	mtime
    }) = deSerialize(blob, headerLayout());
    ::create(time, mtime);
}
