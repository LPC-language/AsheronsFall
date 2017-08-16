# include "pcap.h"


# define ETHER_HEADER_SIZE	14

# define IP_VERSION_IHL		(ETHER_HEADER_SIZE + 0)
# define IP_FLAGS_FRAG		(ETHER_HEADER_SIZE + 6)
# define IP_PROTO		(ETHER_HEADER_SIZE + 9)
# define IP_SRC_ADDR		(ETHER_HEADER_SIZE + 12)
# define IP_DEST_ADDR		(ETHER_HEADER_SIZE + 16)

# define IP_FRAGMENTS		0x3f
# define IP_PROTO_UDP		17
# define IP_HEADER_SIZE		20

# define UDP_SRC_PORT		(ETHER_HEADER_SIZE + 0)
# define UDP_DEST_PORT		(ETHER_HEADER_SIZE + 2)
# define UDP_LENGTH		(ETHER_HEADER_SIZE + 4)

# define UDP_HEADER_SIZE	8

private int time;		/* time of capture */
private float mtime;		/* mtime of capture */
private int origLength;		/* original length */
private string blob;		/* raw packet blob */

/*
 * initialize a captured network packet container
 */
static void create(int time, float mtime, int origLength, string blob)
{
    ::time = time;
    ::mtime = mtime;
    ::origLength = origLength;
    ::blob = blob;
}

/*
 * retrieve an AC packet container from this packet
 */
PacketAC packetAC()
{
    int ihl, length;
    int srcAddr, srcPort;
    int destAddr, destPort;

    /* ethernet header is ignored */

    /*
     * IP layer
     */
    if (strlen(blob) < ETHER_HEADER_SIZE + IP_HEADER_SIZE) {
	return nil;	/* short packet */
    }
    if ((blob[IP_VERSION_IHL] >> 4) != 4) {
	return nil;	/* not IPv4 */
    }
    ihl = (blob[IP_VERSION_IHL] & 0x0f) << 2;
    if (ihl < IP_HEADER_SIZE) {
	return nil;	/* invalid IHL */
    }
    if (blob[IP_PROTO] != IP_PROTO_UDP) {
	return nil;	/* not UDP protocol */
    }
    if (((blob[IP_FLAGS_FRAG] & IP_FRAGMENTS) | blob[IP_FLAGS_FRAG + 1]) != 0) {
	error("IP packet is fragmented");
    }
    srcAddr = (blob[IP_SRC_ADDR] << 24) +
	      (blob[IP_SRC_ADDR + 1] << 16) +
	      (blob[IP_SRC_ADDR + 2] << 8) +
	      blob[IP_SRC_ADDR + 3];
    destAddr = (blob[IP_DEST_ADDR] << 24) +
	       (blob[IP_DEST_ADDR + 1] << 16) +
	       (blob[IP_DEST_ADDR + 2] << 8) +
	       blob[IP_DEST_ADDR + 3];

    /*
     * UDP layer
     */
    length = (blob[ihl + UDP_LENGTH] << 8) + blob[ihl + UDP_LENGTH + 1];
    if (length < UDP_HEADER_SIZE ||
	length > strlen(blob) - ihl - ETHER_HEADER_SIZE) {
	/*
	 * permit the occasional captured packet that's too long, but not the
	 * short ones
	 */
	error("UDP packet has invalid length");
    }
    srcPort = (blob[ihl + UDP_SRC_PORT] << 8) +
	      blob[ihl + UDP_SRC_PORT + 1];
    destPort = (blob[ihl + UDP_DEST_PORT] << 8) +
	       blob[ihl + UDP_DEST_PORT + 1];

    /*
     * AC layer
     */
    return new PacketAC(time, mtime, srcAddr, srcPort, destAddr, destPort,
			blob[ihl + ETHER_HEADER_SIZE + UDP_HEADER_SIZE ..
					 ihl + length + ETHER_HEADER_SIZE - 1]);
}


int time()		{ return time; }
float mtime()		{ return mtime; }
int origLength()	{ return origLength; }
string blob()		{ return blob; }
