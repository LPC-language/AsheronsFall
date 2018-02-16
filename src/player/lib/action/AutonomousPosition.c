# include "GameAction.h"
# include "Position.h"

inherit GameAction;


private Position position;
private int instanceSeq;
private int controlledSeq;
private int teleportSeq;
private int forcedSeq;
private int grounded;

/*
 * initialize AutonomousPosition
 */
static void create(string blob, int offset)
{
    ::create(ACT_AUTONOMOUS_POSITION);
    position = new ClientPosition(blob, offset);
    offset += position->size();
    ({
	offset,
	instanceSeq,
	controlledSeq,
	teleportSeq,
	forcedSeq,
	grounded
    }) = deSerialize(blob, offset, "ssssc");
}


Position position()	{ return position; }
int instanceSeq()	{ return instanceSeq; }
int controlledSeq()	{ return controlledSeq; }
int teleportSeq()	{ return teleportSeq; }
int forcedSeq()		{ return forcedSeq; }
int grounded()		{ return grounded; }
