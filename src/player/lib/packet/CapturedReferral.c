# include "Packet.h"

inherit Referral;


/*
 * create a Referral from a blob
 */
static create(string blob, int offset)
{
    int interfaceCookie, sessionCookie, flags, port, address, something1, zero,
	serverId, something2, something3;

    ({
	offset,
	interfaceCookie,
	sessionCookie,
	flags,
	port,
	address,
	something1,
	zero,
	serverId,
	something2,
	something3
    }) = deSerialize(blob, offset, headerLayout());
    if (zero != 0) {
	error("Zero field not 0");
    }
    ::create(interfaceCookie, sessionCookie, flags, port, address, something1,
	     serverId, something2, something3);
}
