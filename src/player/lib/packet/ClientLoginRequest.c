# include "Packet.h"

inherit LoginRequest;


/*
 * create a LoginRequest from a blob
 */
static void create(string blob, int offset)
{
    string version, account, ticket;
    int size, authType, flags, zero1, zero2, time, ticketLength, len;

    ({
	offset,
	version,
	size,
	authType,
	flags,
	zero1,
	time,
	account,
	zero2,
	ticketLength
    }) = deSerialize(blob, offset, headerLayout());
    len = 20 + 2 + strlen(account);
    len = (len + 3) & ~3;
    if (size != len + ticketLength) {
	error("Bad size");
    }
    if (zero1 != 0) {
	error("Zero1 = " + zero1);
    }
    if (zero2 != 0) {
	error("Zero2 = " + zero2);
    }
    switch (authType) {
    case AUTH_ACCOUNT:
	if (ticketLength != 0) {
	    error("Unexpected ticket");
	}
	break;

    case AUTH_ACCOUNT_TICKET:
	if (ticketLength == 0) {
	    error("Missing ticket");
	}
	({
	    offset,
	    ticketLength
	}) = deSerialize(blob, offset, "S");
	ticket = blob[offset .. offset + ticketLength - 1];
	break;

    default:
	error("Unsupported authentication method");
    }

    ::create(version, flags, time, account, ticket);
}
