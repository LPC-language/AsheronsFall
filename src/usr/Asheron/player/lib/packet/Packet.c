# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


# define HEADER_SIZE	20
# define BADTODD	0xBADD70DD

private int sequence;		/* packet sequence number */
private int flags;		/* bitflags */
private int checksum;		/* badTodd checksum */
private int xorValue;		/* value to xor with body checksum */
private int id;			/* packet ID */
private int time;		/* time */
private int size;		/* size of data */
private int table;		/* packet table */
private mapping data;		/* networkdata blobs in this packet */
private Fragment *fragments;	/* fragments in this packet */

/*
 * Bad Todd checksum calculation
 */
static int badTodd(string blob)
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
    return HEADER_SIZE + size;
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
    packet[0] = serialize(headerLayout(), sequence, flags, 0, id, time, size,
			  table);
    checksum = badTodd(packet[0]) + BADTODD;
    n = 1;
    packetData = map_values(data);
    for (sz = sizeof(packetData), i = 0; i < sz; i++) {
	packet[n++] = packetData[i]->transport();
    }
    bodyChecksum = (sz != 0) ? badTodd(implode(packet[1 .. sz], "")) : 0;
    for (sz = sizeof(fragments), i = 0; i < sz; i++) {
	packet[n] = fragments[i]->transport();
	bodyChecksum += badTodd(packet[n++]);
    }

    blob = implode(packet, "");
    checksum = (checksum + (bodyChecksum ^ xorValue)) & 0xffffffff;
    blob[8] = checksum;
    blob[9] = checksum >> 8;
    blob[10] = checksum >> 16;
    blob[11] = checksum >> 24;

    return blob;
}


/*
 * create a packet with a given header
 */
static void create(int sequence, int flags, int checksum, int xorValue, int id,
		   int time, int table)
{
    ::create(0);

    ::sequence = sequence;
    ::flags = flags;
    ::checksum = checksum;
    ::xorValue = xorValue;
    ::id = id;
    ::time = time;
    ::size = 0;
    ::table = table;
    data = ([ ]);
    fragments = ({ });
}

/*
 * add NetworkData to a packet
 */
int addData(NetworkData item)
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
int addFragment(Fragment fragment)
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
