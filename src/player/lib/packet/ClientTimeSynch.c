# include "Packet.h"

inherit TimeSynch;


/*
 * create a TimeSynch from a blob
 */
static void create(string blob, int offset)
{
    int time;
    float mtime;

    ({
	offset,
	time,
	mtime
    }) = deSerialize(blob, offset, headerLayout());
    ::create(time, mtime);
}
