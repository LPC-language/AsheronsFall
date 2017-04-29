# define NetworkData		object "~/player/lib/packet/NetworkData"
# define Packet			object "~/player/lib/packet/Packet"
# define ClientPacket		object "~/player/lib/packet/ClientPacket"
# define Fragment		object "~/player/lib/packet/Fragment"
# define ClientFragment		object "~/player/lib/packet/ClientFragment"
# define LoginRequest		object "~/player/lib/packet/LoginRequest"
# define ClientLoginRequest	object "~/player/lib/packet/ClientLoginRequest"

/* NetworkData types */
# define NDTYPE_PACKET			1
# define NDTYPE_LOGIN_REQUEST		2

/* packet flags */
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
# define PACKET_CICMD_COMMAND		0x00400000
# define PACKET_TIME_SYNCH		0x01000000
# define PACKET_ECHO_REQUEST		0x02000000
# define PACKET_ECHO_RESPONSE		0x04000000
# define PACKET_FLOW			0x08000000

/* authentication modes */
# define AUTH_ACCOUNT			1
# define AUTH_ACCOUNT_TICKET		2
