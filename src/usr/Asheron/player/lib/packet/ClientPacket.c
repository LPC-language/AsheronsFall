# include "Packet.h"

inherit Packet;


/*
 * create a packet from a blob
 */
static void create(string blob)
{
    int verify, sequence, flags, checksum, id, time, size, table;

    verify = badTodd(blob);
    ({
	blob,
	sequence,
	flags,
	checksum,
	id,
	time,
	size,
	table
    }) = deSerialize(blob, headerLayout());
    if (checksum != ((verify - checksum) & 0xffffffff)) {
	error("Bad checksum");
    }
    if (size != strlen(blob)) {
	error("Bad packet size");
    }
    ::create(sequence, flags, checksum, id, time, table);

    if (flags & PACKET_LOGIN_REQUEST) {
	LoginRequest loginRequest;

	loginRequest = new ClientLoginRequest(blob);
	addData(loginRequest);
	blob = blob[loginRequest->size() ..];
    }
    if (flags & PACKET_CONNECT_RESPONSE) {
	ConnectResponse connectResponse;

	connectResponse = new ClientConnectResponse(blob);
	addData(connectResponse);
	blob = blob[connectResponse->size() ..];
    }
}
