# include "RandSeq.h"
# include "Packet.h"


private int time;		/* time of capture */
private float mtime;		/* mtime of capture */
private int srcAddr, srcPort;	/* source address and port */
private int destAddr, destPort;	/* destination address and port */
private string blob;		/* AC packet blob */

/*
 * initialize a new captured AC packet container
 */
static void create(int time, float mtime, int srcAddr, int srcPort,
		   int destAddr, int destPort, string blob)
{
    ::time = time;
    ::mtime = mtime;
    ::srcAddr = srcAddr;
    ::srcPort = srcPort;
    ::destAddr = destAddr;
    ::destPort = destPort;
    ::blob = blob;
}

/*
 * create an AC packet from the captured data
 */
Packet packet()
{
    return new CapturedPacket(blob);
}


int time()	{ return time; }
float mtime()	{ return mtime; }
int srcAddr()	{ return srcAddr; }
int srcPort()	{ return srcPort; }
int destAddr()	{ return destAddr; }
int destPort()	{ return destPort; }
