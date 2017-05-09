# include "Packet.h"

inherit Packet;


# define HEADER_SIZE	20
# define BADTODD	0xBADD70DD

/*
 * create a packet from a blob
 */
static void create(string blob)
{
    string body;
    int sequence, flags, checksum, id, time, size, table, verify, bodyVerify;

    ({
	body,
	sequence,
	flags,
	checksum,
	id,
	time,
	size,
	table
    }) = deSerialize(blob, headerLayout());
    if (size != strlen(body)) {
	error("Bad packet size");
    }
    ::create(sequence, flags, checksum, id, time, table);
    verify = badTodd(blob[.. HEADER_SIZE - 1]) + BADTODD;

    if (flags & PACKET_LOGIN_REQUEST) {
	LoginRequest loginRequest;

	loginRequest = new ClientLoginRequest(body);
	addData(loginRequest);
	body = body[loginRequest->size() ..];
    }
    if (flags & PACKET_CONNECT_RESPONSE) {
	ConnectResponse connectResponse;

	connectResponse = new ClientConnectResponse(body);
	addData(connectResponse);
	body = body[connectResponse->size() ..];
    }
    bodyVerify = (size != strlen(body)) ?
		  badTodd(blob[HEADER_SIZE ..
			       strlen(blob) - strlen(body) - 1]) : 0;

    verify += bodyVerify;
    if (checksum != ((verify - checksum) & 0xffffffff)) {
	error("Bad checksum");
    }
}
