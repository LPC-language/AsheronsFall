# include "Packet.h"

inherit ConnectRequest;


/*
 * create a ConnectRequest from a blob
 */
static void create(string blob)
{
    int time, interfaceCookie, sessionCookie, clientId, serverSeed, clientSeed,
	something;
    float mtime;

    ({
	blob,
	time,
	mtime,
	interfaceCookie,
	sessionCookie,
	clientId,
	serverSeed,
	clientSeed,
	something
    }) = deSerialize(blob, headerLayout());
    ::create(time, mtime, interfaceCookie, sessionCookie, clientId, serverSeed,
	     clientSeed, something);
}
