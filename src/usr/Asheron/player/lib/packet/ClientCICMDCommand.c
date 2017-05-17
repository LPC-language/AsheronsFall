# include "Packet.h"

inherit CICMDCommand;


/*
 * create a CICMDCommand from a blob
 */
static void create(string blob)
{
    int something1;
    int something2;

    ({
	blob,
	something1,
	something2
    }) = deSerialize(blob, headerLayout());
    ::create(something1, something2);
}
