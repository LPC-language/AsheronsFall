# include "Packet.h"

inherit Referral;


/*
 * create a Referral from a blob
 */
static create(string blob)
{
    int interfaceCookie, sessionCookie, flags, port, address, zero1, zero2,
	serverId, something, seed;

    ({
	blob,
	interfaceCookie,
	sessionCookie,
	flags,
	port,
	address,
	zero1,
	zero2,
	serverId,
	something,
	seed
    }) = deSerialize(blob, headerLayout());
    if ((zero1 | zero2) != 0) {
	error("Zero fields not 0");
    }
    ::create(interfaceCookie, sessionCookie, flags, port, address, serverId,
	     something, seed);
}
