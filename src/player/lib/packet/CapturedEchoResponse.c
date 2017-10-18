# include "Packet.h"

inherit EchoResponse;


/*
 * create an EchoResponse from a blob
 */
static void create(string blob, int offset)
{
    float clientTime, timeDiff;

    ({
	offset,
	clientTime,
	timeDiff
    }) = deSerialize(blob, offset, headerLayout());
    ::create(clientTime, timeDiff);
}
