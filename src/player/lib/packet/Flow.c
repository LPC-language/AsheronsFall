# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private int flowSize;		/* sum of sizes of packets with flowTime */
private int flowTime;		/* time of prev+1 packet received */

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
    return serialize(headerLayout(), flowSize, flowTime);
}

/*
 * create a Flow
 */
static void create(int flowSize, int flowTime)
{
    ::create(PACKET_FLOW);

    ::flowSize = flowSize;
    ::flowTime = flowTime;
}

/*
 * fields
 */
int flowSize()	{ return flowSize; }
int flowTime()	{ return flowTime; }
