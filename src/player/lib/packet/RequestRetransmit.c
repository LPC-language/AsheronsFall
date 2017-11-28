# include "Serialized.h"
# include "Packet.h"

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
    return serializeArray(sequences);
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
