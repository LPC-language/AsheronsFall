# include "Packet.h"

inherit ConnectClose;


/*
 * create a ConnectClose from a blob
 */
static void create(string blob)
{
    int something1, something2;

    ({
	blob,
	something1,
	something2,
    }) = deSerialize(blob, headerLayout());

    ::create(something1, something2);
}
