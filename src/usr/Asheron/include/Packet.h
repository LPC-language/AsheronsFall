# define NETWORKDATA	"/usr/Asheron/player/lib/packet/NetworkData"
# define PACKET		"/usr/Asheron/player/lib/packet/Packet"
# define CLIENTPACKET	"/usr/Asheron/player/lib/packet/ClientPacket"
# define FRAGMENT	"/usr/Asheron/player/lib/packet/Fragment"
# define CLIENTFRAGMENT	"/usr/Asheron/player/lib/packet/ClientFragment"
# define LOGINREQUEST	"/usr/Asheron/player/lib/packet/LoginRequest"
# define CLIENTLOGINREQUEST	\
			"/usr/Asheron/player/lib/packet/ClientLoginRequest"

# define NetworkData	object NETWORKDATA
# define Packet		object PACKET
# define ClientPacket	object CLIENTPACKET
# define Fragment	object FRAGMENT
# define ClientFragment	object CLIENTFRAGMENT
# define LoginRequest	object LOGINREQUEST
# define ClientLoginRequest	\
			object CLIENTLOGINREQUEST

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
