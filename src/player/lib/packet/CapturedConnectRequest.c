# include <Time.h>
# include "Packet.h"

inherit ConnectRequest;


/*
 * create a ConnectRequest from a blob
 */
static void create(string blob, int offset)
{
    Time time;
    int interfaceCookie, sessionCookie, clientId, serverSeed, clientSeed,
	something;

    ({
	offset,
	time,
	interfaceCookie,
	sessionCookie,
	clientId,
	serverSeed,
	clientSeed,
	something
    }) = deSerialize(blob, offset, headerLayout());
    ::create(time, interfaceCookie, sessionCookie, clientId, serverSeed,
	     clientSeed, something);
}
