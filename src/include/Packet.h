# define NetworkData		object "~/player/lib/packet/NetworkData"
# define Packet			object "~/player/lib/packet/Packet"
# define RemotePacket		object "~/player/lib/packet/RemotePacket"
# define CapturedPacket		object "~/player/lib/packet/CapturedPacket"
# define Fragment		object "~/player/lib/packet/Fragment"
# define RemoteFragment		object "~/player/lib/packet/RemoteFragment"
# define ServerSwitch		object "~/player/lib/packet/ServerSwitch"
# define CapturedServerSwitch	object "~/player/lib/packet/CapturedServerSwitch"
# define Referral		object "~/player/lib/packet/Referral"
# define CapturedReferral 	object "~/player/lib/packet/CapturedReferral"
# define RequestRetransmit	object "~/player/lib/packet/RequestRetransmit"
# define RemoteRequestRetransmit object "~/player/lib/packet/RemoteRequestRetransmit"
# define RejectRetransmit	object "~/player/lib/packet/RejectRetransmit"
# define RemoteRejectRetransmit	object "~/player/lib/packet/RemoteRejectRetransmit"
# define AckSequence		object "~/player/lib/packet/AckSequence"
# define RemoteAckSequence	object "~/player/lib/packet/RemoteAckSequence"
# define LoginRequest		object "~/player/lib/packet/LoginRequest"
# define RemoteLoginRequest	object "~/player/lib/packet/RemoteLoginRequest"
# define WorldLoginRequest	object "~/player/lib/packet/WorldLoginRequest"
# define CapturedWorldLoginRequest object "~/player/lib/packet/CapturedWorldLoginRequest"
# define ConnectRequest		object "~/player/lib/packet/ConnectRequest"
# define CapturedConnectRequest	object "~/player/lib/packet/CapturedConnectRequest"
# define ConnectResponse	object "~/player/lib/packet/ConnectResponse"
# define RemoteConnectResponse	object "~/player/lib/packet/RemoteConnectResponse"
# define ConnectError		object "~/player/lib/packet/ConnectError"
# define RemoteConnectError	object "~/player/lib/packet/RemoteConnectError"
# define ConnectClose		object "~/player/lib/packet/ConnectClose"
# define RemoteConnectClose	object "~/player/lib/packet/RemoteConnectClose"
# define CICMDCommand		object "~/player/lib/packet/CICMDCommand"
# define RemoteCICMDCommand	object "~/player/lib/packet/RemoteCICMDCommand"
# define TimeSynch		object "~/player/lib/packet/TimeSynch"
# define RemoteTimeSynch	object "~/player/lib/packet/RemoteTimeSynch"
# define EchoRequest		object "~/player/lib/packet/EchoRequest"
# define RemoteEchoRequest	object "~/player/lib/packet/RemoteEchoRequest"
# define EchoResponse		object "~/player/lib/packet/EchoResponse"
# define CapturedEchoResponse	object "~/player/lib/packet/CapturedEchoResponse"
# define Flow			object "~/player/lib/packet/Flow"
# define RemoteFlow		object "~/player/lib/packet/RemoteFlow"

# define PACKET_HEADER_SIZE		20

/* packet flags */
# define PACKET_TRANSPORT_FLAGS		0x0fffffff
# define PACKET_RETRANSMISSION		0x00000001
# define PACKET_ENCRYPTED_CHECKSUM	0x00000002
# define PACKET_BLOB_FRAGMENTS		0x00000004
# define PACKET_SERVER_SWITCH		0x00000100
# define PACKET_REFERRAL		0x00000800
# define PACKET_REQUEST_RETRANSMIT	0x00001000
# define PACKET_REJECT_RETRANSMIT	0x00002000
# define PACKET_ACK_SEQUENCE		0x00004000
# define PACKET_DISCONNECT		0x00008000
# define PACKET_LOGIN_REQUEST		0x00010000
# define PACKET_WORLD_LOGIN_REQUEST	0x00020000
# define PACKET_CONNECT_REQUEST		0x00040000
# define PACKET_CONNECT_RESPONSE	0x00080000
# define PACKET_CONNECT_ERROR		0x00100000
# define PACKET_CONNECT_CLOSE		0x00200000
# define PACKET_CICMD_COMMAND		0x00400000
# define PACKET_TIME_SYNCH		0x01000000
# define PACKET_ECHO_REQUEST		0x02000000
# define PACKET_ECHO_RESPONSE		0x04000000
# define PACKET_FLOW			0x08000000
# define PACKET_REQUIRED		0x10000000

/* other NetworkData types */
# define ND_DATA			0x00000000
# define ND_PACKET			0x20000000
# define ND_MESSAGE			0x40000000

/* authentication modes */
# define AUTH_ACCOUNT			1
# define AUTH_ACCOUNT_TICKET		2
