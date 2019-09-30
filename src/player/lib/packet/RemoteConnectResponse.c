# include "Packet.h"


inherit ConnectResponse;

/*
 * create a ConnectResponse from a blob
 */
static void create(string blob, int offset)
{
    int interfaceCookie, sessionSeed;

    ({
	offset,
	interfaceCookie,
	sessionSeed
    }) = deSerialize(blob, offset, headerLayout());
    ::create(interfaceCookie, sessionSeed);
}
