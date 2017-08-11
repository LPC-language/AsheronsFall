# include "Packet.h"
# include "RandSeq.h"

inherit ClientPacket;


/*
 * process packet options
 */
static string processOptions(int flags, string body)
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

    if (flags & PACKET_SERVER_SWITCH) {
	ServerSwitch serverSwitch;

	serverSwitch = new ClientServerSwitch(body);
	addData(serverSwitch);
	body = body[serverSwitch->size() ..];
    }
    if (flags & PACKET_REFERRAL) {
	Referral referral;

	referral = new ClientReferral(body);
	addData(referral);
	body = body[referral->size() ..];
    }
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
    if (flags & PACKET_WORLD_LOGIN_REQUEST) {
	WorldLoginRequest worldLoginRequest;

	worldLoginRequest = new ClientWorldLoginRequest(body);
	addData(worldLoginRequest);
	body = body[worldLoginRequest->size() ..];
    }
    if (flags & PACKET_CONNECT_REQUEST) {
	ConnectRequest connectRequest;

	connectRequest = new ClientConnectRequest(body);
	addData(connectRequest);
	body = body[connectRequest->size() ..];
    }
    if (flags & PACKET_CONNECT_RESPONSE) {
	ConnectResponse connectResponse;

	connectResponse = new ClientConnectResponse(body);
	addData(connectResponse);
	body = body[connectResponse->size() ..];
    }
    if (flags & PACKET_CONNECT_ERROR) {
	ConnectError connectError;

	connectError = new ClientConnectError(body);
	addData(connectError);
	body = body[connectError->size() ..];
    }
    if (flags & PACKET_CONNECT_CLOSE) {
	ConnectClose connectClose;

	connectClose = new ClientConnectClose(body);
	addData(connectClose);
	body = body[connectClose->size() ..];
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
    if (flags & PACKET_ECHO_RESPONSE) {
	EchoResponse echoResponse;

	echoResponse = new ClientEchoResponse(body);
	addData(echoResponse);
	body = body[echoResponse->size() ..];
    }
    if (flags & PACKET_FLOW) {
	Flow flow;

	flow = new ClientFlow(body);
	addData(flow);
	body = body[flow->size() ..];
    }

    return body;
}
