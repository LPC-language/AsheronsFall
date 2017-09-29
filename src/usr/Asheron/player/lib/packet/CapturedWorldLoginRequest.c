# include "Packet.h"

inherit WorldLoginRequest;


/*
 * create a WorldLoginRequest from a blob
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
