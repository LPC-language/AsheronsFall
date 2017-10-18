# include "Packet.h"

inherit WorldLoginRequest;


/*
 * create a WorldLoginRequest from a blob
 */
static void create(string blob, int offset)
{
    int interfaceCookie, sessionCookie;

    ({
	offset,
	interfaceCookie,
	sessionCookie
    }) = deSerialize(blob, offset, headerLayout());
    ::create(interfaceCookie, sessionCookie);
}
