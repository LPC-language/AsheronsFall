# include "Packet.h"

inherit RejectRetransmit;


/*
 * create a RejectTransmit from a blob
 */
static void create(string blob, int offset)
{
    ::create(deSerializeArray(blob, offset)[1]);
}
