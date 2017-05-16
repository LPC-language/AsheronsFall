# include "Packet.h"

inherit Packet;


# define HEADER_SIZE	20
# define BADTODD	0xBADD70DD

/*
 * create a packet from a blob
 */
static void create(string blob, object randGen)
{
    string body;
    int sequence, flags, checksum, id, time, size, table, verify, bodyVerify,
	xorValue;

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
    if (flags & PACKET_ENCRYPTED_CHECKSUM) {
	xorValue = randGen->randSequence(sequence);
    }
    ::create(sequence, flags, checksum, xorValue, id, time, table);
    verify = badTodd(blob[.. HEADER_SIZE - 1]) + BADTODD;

    if (flags & PACKET_ACK_SEQUENCE) {
	AckSequence ackSequence;

	ackSequence = new ClientAckSequence(body);
	addData(ackSequence);
	body = body[ackSequence->size() ..];
    }
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
    if (flags & PACKET_TIME_SYNCH) {
	TimeSynch timeSynch;

	timeSynch = new ClientTimeSynch(body);
	addData(timeSynch);
	body = body[timeSynch->size() ..];
    }
    if (flags & PACKET_FLOW) {
	Flow flow;

	flow = new ClientFlow(body);
	addData(flow);
	body = body[flow->size() ..];
    }
    if (size != strlen(body)) {
	bodyVerify = badTodd(blob[HEADER_SIZE ..
				  strlen(blob) - strlen(body) - 1]);
    }

    if (flags & PACKET_BLOB_FRAGMENTS) {
	Fragment fragment;
	int size;

	do {
	    fragment = new ClientFragment(body);
	    addFragment(fragment);
	    size = fragment->size();
	    bodyVerify += badTodd(body[.. size - 1]);
	    body = body[size ..];
	} while (strlen(body) != 0);
    }

    if (body != "") {
	error("Garbage in packet");
    }

    verify += bodyVerify ^ xorValue;
    if (checksum != ((verify - checksum) & 0xffffffff)) {
	error("Bad checksum");
    }
}
