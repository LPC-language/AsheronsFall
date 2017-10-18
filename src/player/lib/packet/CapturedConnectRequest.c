# include "Packet.h"

inherit ConnectRequest;


/*
 * create a ConnectRequest from a blob
 */
static void create(string blob, int offset)
{
    int time, interfaceCookie, sessionCookie, clientId, serverSeed, clientSeed,
	something;
    float mtime;

    ({
	offset,
	time,
	mtime,
	interfaceCookie,
	sessionCookie,
	clientId,
	serverSeed,
	clientSeed,
	something
    }) = deSerialize(blob, offset, headerLayout());
    ::create(time, mtime, interfaceCookie, sessionCookie, clientId, serverSeed,
	     clientSeed, something);
}
