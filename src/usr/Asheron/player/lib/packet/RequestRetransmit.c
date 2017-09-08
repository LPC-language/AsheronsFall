# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int *sequences;	/* sequence numbers of packets to retransmit */

/*
 * layout of RequestRetransmit header
 */
static string headerLayout()
{
    return "i";
}

/*
 * size of the entire blob
 */
int size()
{
    return 4 * (sizeof(sequences) + 1);
}

/*
 * export as a blob
 */
string transport()
{
    string *serialized;
    int sz, i;

    sz = sizeof(sequences);
    serialized = allocate(sz);
    for (i = 0; i < sz; i++) {
	serialized[i] = serialize("i", sequences[i]);
    }
    return serialize(headerLayout(), sz) + implode(serialized, "");
}

/*
 * create a RequestRetransmit
 */
static void create(int *sequences)
{
    ::create(PACKET_REQUEST_RETRANSMIT);

    ::sequences = sequences;
}

/*
 * fields
 */
int *sequences()	{ return sequences[..]; }
