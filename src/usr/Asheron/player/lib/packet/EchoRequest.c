# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private float clientTime;	/* client time */

/*
 * layout of EchoRequest
 */
static string headerLayout()
{
    return "f";
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
    return serialize(headerLayout(), clientTime);
}

/*
 * create an EchoRequest
 */
static void create(float clientTime)
{
    ::create(PACKET_ECHO_REQUEST);

    ::clientTime = clientTime;
}

/*
 * fields
 */
float clientTime()	{ return clientTime; }
