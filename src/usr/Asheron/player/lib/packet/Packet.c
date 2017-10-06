# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int sequence;		/* packet sequence number */
private int flags;		/* bitflags */
private int headerChecksum;	/* header checksum */
private int bodyChecksum;	/* body checksum */
private int checksum;		/* packet checksum */
private int xorValue;		/* value to xor with body checksum */
private int id;			/* server/client ID */
private int time;		/* time */
private int size;		/* size of data */
private int table;		/* packet table */
private mapping data;		/* networkdata blobs in this packet */
private Fragment *fragments;	/* fragments in this packet */

/*
 * Bad Todd checksum calculation
 */
private int badTodd(string blob)
{
    int len, tail, checksum, c0, c1, c2, c3, i;

    len = strlen(blob);

    /* include length */
    checksum = len << 16;
    c0 = c1 = c2 = c3 = 0;

    tail = len & 3;
    len -= tail;

    /* little endian */
    for (i = 0; i < len; i += 4) {
	c0 += blob[i];
	c1 += blob[i + 1];
	c2 += blob[i + 2];
	c3 += blob[i + 3];
    }

    /* big endian */
    switch (tail) {
    case 3:
	c3 += blob[i];
	c2 += blob[i + 1];
	c1 += blob[i + 2];
	break;

    case 2:
	c3 += blob[i];
	c2 += blob[i + 1];
	break;

    case 1:
	c3 += blob[i];
	break;
    }

    return checksum + c0 + (c1 << 8) + (c2 << 16) + (c3 << 24);
}

/*
 * compute header checksum
 */
static void setHeaderChecksum(string header)
{
    headerChecksum = badTodd(header) - checksum + 0xBADD70DD;
    bodyChecksum = 0;
}

/*
 * accumulate body checksum
 */
static void addBodyChecksum(string body)
{
    bodyChecksum += badTodd(body);
}

/*
 * compute checksum for entire packet
 */
int computeChecksum()
{
    return (headerChecksum + (bodyChecksum ^ xorValue)) & 0xffffffff;
}


/*
 * packet header layout string
 */
static string headerLayout()
{
    return "iiissss";
}

/*
 * size of the entire packet
 */
int size()
{
    return PACKET_HEADER_SIZE + size;
}

/*
 * transport a packet as a binary blob
 */
string transport()
{
    string *packet, blob;
    int n, sz, i, bodyChecksum;
    NetworkData *packetData;

    packet = allocate(1 + map_sizeof(data) + sizeof(fragments));
    packet[0] = serialize(headerLayout(), sequence,
			  flags & PACKET_TRANSPORT_FLAGS, checksum, id, time,
			  size, table);
    setHeaderChecksum(packet[0]);
    n = 1;
    packetData = map_values(data);
    for (sz = sizeof(packetData), i = 0; i < sz; i++) {
	packet[n++] = packetData[i]->transport();
    }
    if (sz != 0) {
	addBodyChecksum(implode(packet[1 .. sz], ""));
    }
    for (sz = sizeof(fragments), i = 0; i < sz; i++) {
	packet[n] = fragments[i]->transport();
	addBodyChecksum(packet[n++]);
    }

    blob = packet[0];
    checksum = computeChecksum();
    blob[8] = checksum;
    blob[9] = checksum >> 8;
    blob[10] = checksum >> 16;
    blob[11] = checksum >> 24;
    packet[0] = blob;

    return implode(packet, "");
}


/*
 * create a packet with a given header
 */
static void create(int checksum, int id, int table)
{
    ::create(ND_PACKET);

    sequence = 0;
    ::checksum = checksum;
    ::id = id;
    time = 0;
    size = 0;
    ::table = table;
    data = ([ ]);
    fragments = ({ });
}

/*
 * make top-level datastructures in this packet unique
 */
static void copy()
{
    data = data[..];
    fragments = fragments[..];
}

/*
 * create a duplicate of this packet
 */
Packet duplicate()
{
    return copy_object();
}

/*
 * set the sequence field
 */
void setSequence(int sequence)
{
    ::sequence = sequence;
}

/*
 * Prepare to retransmit packet.  All that changes in retransmitted packets
 * is the time, the checksum and 1 bit in the flags; i.e. flow is not updated.
 */
void setRetransmission()
{
    flags |= PACKET_RETRANSMISSION;
}

/*
 * set EncryptedChecksum flag
 */
void setEncryptedChecksum()
{
    flags |= PACKET_ENCRYPTED_CHECKSUM;
}

/*
 * set Disconnect flag
 */
void setDisconnect()
{
    flags |= PACKET_DISCONNECT;
}

/*
 * set Required flag
 */
void setRequired()
{
    flags |= PACKET_REQUIRED;
}

/*
 * set value to "encrypt" checksum with
 */
void setXorValue(int xorValue)
{
    ::xorValue = xorValue;
}

/*
 * set time field
 */
void setTime(int time)
{
    ::time = time;
}

/*
 * add NetworkData to a packet
 */
void addData(NetworkData item)
{
    int type;

    type = item->networkDataType();
    flags |= type;
    data[type] = item;
    size += item->size();
}

/*
 * add a fragment to a packet
 */
void addFragment(Fragment fragment)
{
    flags |= PACKET_BLOB_FRAGMENTS;
    fragments += ({ fragment });
    size += fragment->size();
}

/*
 * fields
 */
int sequence()			{ return sequence; }
int flags()			{ return flags; }
int checksum()			{ return checksum; }
int xorValue()			{ return xorValue; }
int id()			{ return id; }
int time()			{ return time; }
int table()			{ return table; }
NetworkData data(int type)	{ return data[type]; }
int numFragments()		{ return sizeof(fragments); }
Fragment fragment(int n)	{ return fragments[n]; }
