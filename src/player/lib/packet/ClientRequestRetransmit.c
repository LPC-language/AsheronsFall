# include "Packet.h"

inherit RequestRetransmit;


/*
 * create a RequestTransmit from a blob
 */
static void create(string blob, int offset)
{
    ::create(deSerializeArray(blob, offset)[1]);
}
