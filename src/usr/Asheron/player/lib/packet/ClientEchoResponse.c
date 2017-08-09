# include "Packet.h"

inherit EchoResponse;


/*
 * create an EchoResponse from a blob
 */
static void create(string blob)
{
    float clientTime, timeDiff;

    ({
	blob,
	clientTime,
	timeDiff
    }) = deSerialize(blob, headerLayout());
    ::create(clientTime, timeDiff);
}
