# include "Packet.h"

inherit Packet;


/*
 * process packet options
 */
static int processOptions(int flags, string blob, int offset)
{
    if (flags & ~(PACKET_RETRANSMISSION | PACKET_ENCRYPTED_CHECKSUM |
		  PACKET_BLOB_FRAGMENTS | PACKET_REQUEST_RETRANSMIT |
		  PACKET_REJECT_RETRANSMIT | PACKET_ACK_SEQUENCE |
		  PACKET_DISCONNECT | PACKET_LOGIN_REQUEST |
		  PACKET_CONNECT_RESPONSE | PACKET_CONNECT_ERROR |
		  PACKET_CONNECT_CLOSE | PACKET_CICMD_COMMAND |
		  PACKET_TIME_SYNCH | PACKET_ECHO_REQUEST | PACKET_FLOW)) {
	error("Bad packet flags: " + flags);
    }

    if (flags & PACKET_RETRANSMISSION) {
	setRetransmission();
    }
    if (flags & PACKET_ENCRYPTED_CHECKSUM) {
	setEncryptedChecksum();
    }
    if (flags & PACKET_REQUEST_RETRANSMIT) {
	RequestRetransmit requestRetransmit;

	requestRetransmit = new ClientRequestRetransmit(blob, offset);
	addData(requestRetransmit);
	offset += requestRetransmit->size();
    }
    if (flags & PACKET_REJECT_RETRANSMIT) {
	RejectRetransmit rejectRetransmit;

	rejectRetransmit = new ClientRejectRetransmit(blob, offset);
	addData(rejectRetransmit);
	offset += rejectRetransmit->size();
    }
    if (flags & PACKET_ACK_SEQUENCE) {
	AckSequence ackSequence;

	ackSequence = new ClientAckSequence(blob, offset);
	addData(ackSequence);
	offset += ackSequence->size();
    }
    if (flags & PACKET_DISCONNECT) {
	setDisconnect();
    }
    if (flags & PACKET_LOGIN_REQUEST) {
	LoginRequest loginRequest;

	loginRequest = new ClientLoginRequest(blob, offset);
	addData(loginRequest);
	offset += loginRequest->size();
    }
    if (flags & PACKET_CONNECT_RESPONSE) {
	ConnectResponse connectResponse;

	connectResponse = new ClientConnectResponse(blob, offset);
	addData(connectResponse);
	offset += connectResponse->size();
    }
    if (flags & PACKET_CONNECT_ERROR) {
	ConnectError connectError;

	connectError = new ClientConnectError(blob, offset);
	addData(connectError);
	offset += connectError->size();
    }
    if (flags & PACKET_CONNECT_CLOSE) {
	ConnectClose connectClose;

	connectClose = new ClientConnectClose(blob, offset);
	addData(connectClose);
	offset += connectClose->size();
    }
    if (flags & PACKET_CICMD_COMMAND) {
	CICMDCommand command;

	command = new ClientCICMDCommand(blob, offset);
	addData(command);
	offset += command->size();
    }
    if (flags & PACKET_TIME_SYNCH) {
	TimeSynch timeSynch;

	timeSynch = new ClientTimeSynch(blob, offset);
	addData(timeSynch);
	offset += timeSynch->size();
    }
    if (flags & PACKET_ECHO_REQUEST) {
	EchoRequest echoRequest;

	echoRequest = new ClientEchoRequest(blob, offset);
	addData(echoRequest);
	offset += echoRequest->size();
    }
    if (flags & PACKET_FLOW) {
	Flow flow;

	flow = new ClientFlow(blob, offset);
	addData(flow);
	offset += flow->size();
    }

    return offset;
}

/*
 * create a packet from a blob
 */
static void create(string blob)
{
    int offset, sequence, flags, checksum, id, time, size, table;

    /*
     * process packet header
     */
    ({
	offset,
	sequence,
	flags,
	checksum,
	id,
	time,
	size,
	table
    }) = deSerialize(blob, 0, headerLayout());
    if (strlen(blob) != offset + size) {
	error("Bad packet size");
    }
    ::create(checksum, id, table);
    setSequence(sequence);
    setTime(time);
    setHeaderChecksum(blob);

    offset = processOptions(flags, blob, offset);
    if (offset != PACKET_HEADER_SIZE) {
	addBodyChecksum(blob, PACKET_HEADER_SIZE, offset - PACKET_HEADER_SIZE);
    }

    /*
     * handle fragments
     */
    if (flags & PACKET_BLOB_FRAGMENTS) {
	Fragment fragment;
	int size;

	do {
	    fragment = new ClientFragment(blob, offset);
	    addFragment(fragment);
	    size = fragment->size();
	    addBodyChecksum(blob, offset, size);
	    offset += size;
	} while (strlen(blob) != offset);
    }

    if (strlen(blob) != offset) {
	error("Garbage in packet");
    }
}
