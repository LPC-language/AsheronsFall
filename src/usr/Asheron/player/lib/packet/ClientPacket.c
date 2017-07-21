# include "Packet.h"
# include "RandSeq.h"

inherit Packet;


# define HEADER_SIZE	20
# define BADTODD	0xBADD70DD

/*
 * create a packet from a blob
 */
static void create(string blob, RandSeq randGen)
{
    string body;
    int sequence, flags, checksum, id, time, size, table, verify, bodyVerify,
	xorValue;

    /*
     * process packet header
     */
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
    if (flags & ~(PACKET_RETRANSMISSION | PACKET_ENCRYPTED_CHECKSUM |
		  PACKET_BLOB_FRAGMENTS | PACKET_REQUEST_RETRANSMIT |
		  PACKET_REJECT_RETRANSMIT | PACKET_ACK_SEQUENCE |
		  PACKET_DISCONNECT | PACKET_LOGIN_REQUEST |
		  PACKET_CONNECT_RESPONSE | PACKET_CICMD_COMMAND |
		  PACKET_TIME_SYNCH | PACKET_ECHO_REQUEST | PACKET_FLOW)) {
	error("Bad packet flags: " + flags);
    }
    if (flags & PACKET_ENCRYPTED_CHECKSUM) {
	xorValue = randGen->rand(sequence - 2);
    }
    ::create(sequence, flags, checksum, xorValue, id, time, table);
    verify = badTodd(blob[.. HEADER_SIZE - 1]) + BADTODD;

    /*
     * process packet options
     */
    if (flags & PACKET_REQUEST_RETRANSMIT) {
	RequestRetransmit requestRetransmit;

	requestRetransmit = new ClientRequestRetransmit(body);
	addData(requestRetransmit);
	body = body[requestRetransmit->size() ..];
    }
    if (flags & PACKET_REJECT_RETRANSMIT) {
	RejectRetransmit rejectRetransmit;

	rejectRetransmit = new ClientRejectRetransmit(body);
	addData(rejectRetransmit);
	body = body[rejectRetransmit->size() ..];
    }
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
    if (flags & PACKET_CICMD_COMMAND) {
	CICMDCommand command;

	command = new ClientCICMDCommand(body);
	addData(command);
	body = body[command->size() ..];
    }
    if (flags & PACKET_TIME_SYNCH) {
	TimeSynch timeSynch;

	timeSynch = new ClientTimeSynch(body);
	addData(timeSynch);
	body = body[timeSynch->size() ..];
    }
    if (flags & PACKET_ECHO_REQUEST) {
	EchoRequest echoRequest;

	echoRequest = new ClientEchoRequest(body);
	addData(echoRequest);
	body = body[echoRequest->size() ..];
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

    /*
     * handle fragments
     */
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

    /*
     * verify checksum
     */
    verify += bodyVerify ^ xorValue;
    if (checksum != ((verify - checksum) & 0xffffffff)) {
	error("Bad checksum");
    }
}
