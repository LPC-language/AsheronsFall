# include <Time.h>
# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private Time time;	/* time to sync with */

/*
 * layout of TimeSync
 */
static string headerLayout()
{
    return "D";
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
static void create(Time time)
{
    ::create(PACKET_TIME_SYNCH);

    ::time = time;
}

/*
 * fields
 */
Time time()	{ return time; }
