# include "Packet.h"

inherit ConnectError;


/*
 * create a ConnectError from a blob
 */
static void create(string blob, int offset)
{
    int something1, something2;

    ({
	offset,
	something1,
	something2,
    }) = deSerialize(blob, offset, headerLayout());

    ::create(something1, something2);
}
