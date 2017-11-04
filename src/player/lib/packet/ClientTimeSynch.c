# include <Time.h>
# include "Packet.h"

inherit TimeSynch;


/*
 * create a TimeSynch from a blob
 */
static void create(string blob, int offset)
{
    Time time;

    ({
	offset,
	time,
    }) = deSerialize(blob, offset, headerLayout());
    ::create(time);
}
