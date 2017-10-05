# include "Packet.h"


inherit ConnectResponse;

/*
 * create a ConnectResponse from a blob
 */
static void create(string blob)
{
    int interfaceCookie, sessionSeed;

    ({
	blob,
	interfaceCookie,
	sessionSeed
    }) = deSerialize(blob, headerLayout());
    ::create(interfaceCookie, sessionSeed);
}
