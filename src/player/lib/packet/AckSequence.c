# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private int sequence;		/* sequence number ack'ed */

/*
 * layout of sequence number
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
    return 4;
}

/*
 * export as a blob
 */
string transport()
{
    return serialize(headerLayout(), sequence);
}

/*
 * create an AckSequence
 */
static void create(int sequence)
{
    ::create(PACKET_ACK_SEQUENCE);

    ::sequence = sequence;
}

/*
 * fields
 */
int sequence()		{ return sequence; }
