# include "Packet.h"

inherit EchoRequest;


/*
 * create an EchoRequest from a blob
 */
static void create(string blob)
{
    float clientTime;

    ({
	blob,
	clientTime,
    }) = deSerialize(blob, headerLayout());

    ::create(clientTime);
}
