# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private int *sequences;	/* sequence numbers of packets to reject */

/*
 * layout of RejectRetransmit header
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
 * create a RejectRetransmit
 */
static void create(int *sequences)
{
    ::create(PACKET_REJECT_RETRANSMIT);

    ::sequences = sequences;
}

/*
 * fields
 */
int *sequences()	{ return sequences[..]; }
