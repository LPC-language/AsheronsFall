# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


# define HEADER_SIZE	20

private int sequence;		/* packet sequence number */
private int flags;		/* bitflags */
private int checksum;		/* badTodd checksum */
private int id;			/* packet ID */
private int time;		/* time */
private int size;		/* size of data */
private int table;		/* packet table */
private NetworkData *data;	/* networkdata blobs in this packet */
private Fragment *fragments;	/* fragments in this packet */

/*
 * Bad Todd checksum calculation
 */
static int badTodd(string data)
{
    int len, tail, checksum, c0, c1, c2, c3, i;

    len = strlen(data);

    /* include length */
    checksum = len << 16;
    c0 = c1 = c2 = c3 = 0;

    tail = len & 3;
    len -= tail;

    /* little endian */
    for (i = 0; i < len; i += 4) {
	c0 += data[i];
	c1 += data[i + 1];
	c2 += data[i + 2];
	c3 += data[i + 3];
    }

    /* big endian */
    switch (tail) {
    case 3:
	c3 += data[i];
	c2 += data[i + 1];
	c1 += data[i + 2];
	break;

    case 2:
	c3 += data[i];
	c2 += data[i + 1];
	break;

    case 1:
	c3 += data[i];
	break;
    }

    return (checksum + c0 + (c1 << 8) + (c2 << 16) + (c3 << 24) + 0xBADD70DD) &
	   0xffffffff;
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
    int n, size, i;

    packet = allocate(1 + sizeof(data) + sizeof(fragments));
    packet[0] = serialize(headerLayout(), sequence, flags, 0, id, time, size,
			  table);
    n = 1;
    for (size = sizeof(data), i = 0; i < size; i++) {
	packet[n++] = data[i]->transport();
    }
    for (size = sizeof(fragments), i = 0; i < size; i++) {
	packet[n++] = fragments[i]->transport();
    }

    blob = implode(packet, "");
    checksum = badTodd(blob);
    blob[8] = checksum;
    blob[9] = checksum >> 8;
    blob[10] = checksum >> 16;
    blob[11] = checksum >> 24;

    return blob;
}


/*
 * create a packet with a given header
 */
static void create(int sequence, int flags, int checksum, int id, int time,
		   int table)
{
    ::create(NDTYPE_PACKET);

    ::sequence = sequence;
    ::flags = flags;
    ::checksum = checksum;
    ::id = id;
    ::time = time;
    ::size = 0;
    ::table = table;
    data = ({ });
    fragments = ({ });
}

/*
 * add NetworkData to a packet
 */
int addData(NetworkData item)
{
    data += ({ item });
    size += item->size();
}

/*
 * add a fragment to a packet
 */
int addFragment(Fragment fragment)
{
    fragments += ({ fragment });
    size += fragment->size();
}

/*
 * fields
 */
int sequence()		{ return sequence; }
int flags()		{ return flags; }
int checksum()		{ return checksum; }
int id()		{ return id; }
int time()		{ return time; }
int table()		{ return table; }
NetworkData *data()	{ return data[..]; }
Fragment *fragments()	{ return fragments[..]; }
