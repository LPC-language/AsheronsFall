# include "RandSeq.h"
# include "Packet.h"

inherit RemotePacket;


/*
 * process packet options
 */
static int processOptions(int flags, string blob, int offset)
{
    if (flags & ~(PACKET_RETRANSMISSION | PACKET_ENCRYPTED_CHECKSUM |
		  PACKET_BLOB_FRAGMENTS | PACKET_SERVER_SWITCH |
		  PACKET_REFERRAL | PACKET_REQUEST_RETRANSMIT |
		  PACKET_REJECT_RETRANSMIT | PACKET_ACK_SEQUENCE |
		  PACKET_DISCONNECT | PACKET_LOGIN_REQUEST |
		  PACKET_WORLD_LOGIN_REQUEST | PACKET_CONNECT_REQUEST |
		  PACKET_CONNECT_RESPONSE | PACKET_CONNECT_ERROR |
		  PACKET_CONNECT_CLOSE | PACKET_CICMD_COMMAND |
		  PACKET_TIME_SYNCH | PACKET_ECHO_REQUEST |
		  PACKET_ECHO_RESPONSE | PACKET_FLOW)) {
	error("Bad packet flags: " + flags);
    }

    if (flags & PACKET_RETRANSMISSION) {
	setRetransmission();
    }
    if (flags & PACKET_ENCRYPTED_CHECKSUM) {
	setEncryptedChecksum();
    }
    if (flags & PACKET_SERVER_SWITCH) {
	ServerSwitch serverSwitch;

	serverSwitch = new CapturedServerSwitch(blob, offset);
	addData(serverSwitch);
	offset += serverSwitch->size();
    }
    if (flags & PACKET_REFERRAL) {
	Referral referral;

	referral = new CapturedReferral(blob, offset);
	addData(referral);
	offset += referral->size();
    }
    if (flags & PACKET_REQUEST_RETRANSMIT) {
	RequestRetransmit requestRetransmit;

	requestRetransmit = new RemoteRequestRetransmit(blob, offset);
	addData(requestRetransmit);
	offset += requestRetransmit->size();
    }
    if (flags & PACKET_REJECT_RETRANSMIT) {
	RejectRetransmit rejectRetransmit;

	rejectRetransmit = new RemoteRejectRetransmit(blob, offset);
	addData(rejectRetransmit);
	offset += rejectRetransmit->size();
    }
    if (flags & PACKET_ACK_SEQUENCE) {
	AckSequence ackSequence;

	ackSequence = new RemoteAckSequence(blob, offset);
	addData(ackSequence);
	offset += ackSequence->size();
    }
    if (flags & PACKET_DISCONNECT) {
	setDisconnect();
    }
    if (flags & PACKET_LOGIN_REQUEST) {
	LoginRequest loginRequest;

	loginRequest = new RemoteLoginRequest(blob, offset);
	addData(loginRequest);
	offset += loginRequest->size();
    }
    if (flags & PACKET_WORLD_LOGIN_REQUEST) {
	WorldLoginRequest worldLoginRequest;

	worldLoginRequest = new CapturedWorldLoginRequest(blob, offset);
	addData(worldLoginRequest);
	offset += worldLoginRequest->size();
    }
    if (flags & PACKET_CONNECT_REQUEST) {
	ConnectRequest connectRequest;

	connectRequest = new CapturedConnectRequest(blob, offset);
	addData(connectRequest);
	offset += connectRequest->size();
    }
    if (flags & PACKET_CONNECT_RESPONSE) {
	ConnectResponse connectResponse;

	connectResponse = new RemoteConnectResponse(blob, offset);
	addData(connectResponse);
	offset += connectResponse->size();
    }
    if (flags & PACKET_CONNECT_ERROR) {
	ConnectError connectError;

	connectError = new RemoteConnectError(blob, offset);
	addData(connectError);
	offset += connectError->size();
    }
    if (flags & PACKET_CONNECT_CLOSE) {
	ConnectClose connectClose;

	connectClose = new RemoteConnectClose(blob, offset);
	addData(connectClose);
	offset += connectClose->size();
    }
    if (flags & PACKET_CICMD_COMMAND) {
	CICMDCommand command;

	command = new RemoteCICMDCommand(blob, offset);
	addData(command);
	offset += command->size();
    }
    if (flags & PACKET_TIME_SYNCH) {
	TimeSynch timeSynch;

	timeSynch = new RemoteTimeSynch(blob, offset);
	addData(timeSynch);
	offset += timeSynch->size();
    }
    if (flags & PACKET_ECHO_REQUEST) {
	EchoRequest echoRequest;

	echoRequest = new RemoteEchoRequest(blob, offset);
	addData(echoRequest);
	offset += echoRequest->size();
    }
    if (flags & PACKET_ECHO_RESPONSE) {
	EchoResponse echoResponse;

	echoResponse = new CapturedEchoResponse(blob, offset);
	addData(echoResponse);
	offset += echoResponse->size();
    }
    if (flags & PACKET_FLOW) {
	Flow flow;

	flow = new RemoteFlow(blob, offset);
	addData(flow);
	offset += flow->size();
    }

    return offset;
}
