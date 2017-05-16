# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private float time;	/* time to sync with */

/*
 * layout of time
 */
static string headerLayout()
{
    return "d";
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
    return serialize(headerLayout(), time);
}

/*
 * create a TimeSynch
 */
static void create(float time)
{
    ::create(PACKET_TIME_SYNCH);

    ::time = time;
}

/*
 * fields
 */
float time()	{ return time; }
