# include "Packet.h"


inherit ConnectResponse;

/*
 * create a ConnectResponse from a blob
 */
static void create(string blob)
{
    int interfaceCookie, sessionCookie;

    ({
	blob,
	interfaceCookie,
	sessionCookie
    }) = deSerialize(blob, headerLayout());
    ::create(interfaceCookie, sessionCookie);
}
