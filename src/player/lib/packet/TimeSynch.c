# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private int time;	/* time to sync with */
private float mtime;	/* mtime to sync with */

/*
 * layout of TimeSync
 */
static string headerLayout()
{
    return "DD";
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
    return serialize(headerLayout(), time, mtime);
}

/*
 * create a TimeSynch
 */
static void create(int time, float mtime)
{
    ::create(PACKET_TIME_SYNCH);

    ::time = time;
    ::mtime = mtime;
}

/*
 * fields
 */
int time()	{ return time; }
float mtime()	{ return mtime; }
