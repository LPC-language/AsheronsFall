# include "Packet.h"
# include "Serialized.h"

inherit NetworkData;
inherit Serialized;


private int prevSize;		/* size of prev+1 packet received */
private int prevTime;		/* time of prev+1 packet received */

/*
 * layout of flow
 */
static string headerLayout()
{
    return "is";
}

/*
 * size of the entire blob
 */
int size()
{
    return 6;
}

/*
 * export as a blob
 */
string transport()
{
    return serialize(headerLayout(), prevSize, prevTime);
}

/*
 * create a Flow
 */
static void create(int prevSize, int prevTime)
{
    ::create(PACKET_FLOW);

    ::prevSize = prevSize;
    ::prevTime = prevTime;
}

/*
 * fields
 */
int prevSize()	{ return prevSize; }
int prevTime()	{ return prevTime; }
