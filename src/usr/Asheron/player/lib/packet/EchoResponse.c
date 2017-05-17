# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private float clientTime;	/* client time */
private float timeDiff;		/* server time - client time */

/*
 * layout of EchoResponse
 */
static string headerLayout()
{
    return "ff";
}

/*
 * size of the entire blob
 */
int size()
{
    return 8;
}

/*
 * export as a blob
 */
string transport()
{
    return serialize(headerLayout(), clientTime, timeDiff);
}

/*
 * create an EchoResponse
 */
static void create(float clientTime, float timeDiff)
{
    ::create(PACKET_ECHO_RESPONSE);

    ::clientTime = clientTime;
    ::timeDiff = timeDiff;
}

/*
 * fields
 */
float clientTime()	{ return clientTime; }
float timeDiff()	{ return timeDiff; }
