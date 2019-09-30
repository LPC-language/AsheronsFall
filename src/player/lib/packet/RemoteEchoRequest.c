# include "Packet.h"

inherit EchoRequest;


/*
 * create an EchoRequest from a blob
 */
static void create(string blob, int offset)
{
    float clientTime;

    ({
	offset,
	clientTime,
    }) = deSerialize(blob, offset, headerLayout());

    ::create(clientTime);
}
