# include "GameAction.h"
# include "Position.h"
# include "motion.h"

inherit GameAction;


# define FLAG_CURRENT_HOLD_KEY		0x0001
# define FLAG_CURRENT_STANCE		0x0002
# define FLAG_FORWARD_COMMAND		0x0004
# define FLAG_FORWARD_HOLD_KEY		0x0008
# define FLAG_FORWARD_SPEED		0x0010
# define FLAG_SIDESTEP_COMMAND		0x0020
# define FLAG_SIDESTEP_HOLD_KEY		0x0040
# define FLAG_SIDESTEP_SPEED		0x0080
# define FLAG_TURN_COMMAND		0x0100
# define FLAG_TURN_HOLD_KEY		0x0200
# define FLAG_TURN_SPEED		0x0400

private int currentHoldKey;
private int currentStance;
private int forwardCommand;
private int forwardHoldKey;
private float forwardSpeed;
private int sidestepCommand;
private int sidestepHoldKey;
private float sidestepSpeed;
private int turnCommand;
private int turnHoldKey;
private float turnSpeed;
private mixed **motionCommands;
private Position position;
private int instanceSeq;
private int controlSeq;
private int teleportSeq;
private int forcedSeq;
private int flags;

/*
 * create MoveToState
 */
static void create(string blob, int offset)
{
    int flags, n, i;
    mixed *motionCommand;

    ::create(ACT_MOVE_TO_STATE);

    ({ offset, flags }) = deSerialize(blob, offset, "i");

    /*
     * flags
     */
    if (flags & FLAG_CURRENT_HOLD_KEY) {
	({ offset, currentHoldKey }) = deSerialize(blob, offset, "i");
    }
    if (flags & FLAG_CURRENT_STANCE) {
	({ offset, currentStance }) = deSerialize(blob, offset, "i");
    } else {
	currentStance = MOTION_NONCOMBAT;
    }
    if (flags & FLAG_FORWARD_COMMAND) {
	({ offset, forwardCommand }) = deSerialize(blob, offset, "i");
    } else {
	forwardCommand = MOTION_READY;
    }
    if (flags & FLAG_FORWARD_HOLD_KEY) {
	({ offset, forwardHoldKey }) = deSerialize(blob, offset, "i");
    }
    if (flags & FLAG_FORWARD_SPEED) {
	({ offset, forwardSpeed }) = deSerialize(blob, offset, "f");
    } else {
	forwardSpeed = 1.0;
    }
    if (flags & FLAG_SIDESTEP_COMMAND) {
	({ offset, sidestepCommand }) = deSerialize(blob, offset, "i");
    }
    if (flags & FLAG_SIDESTEP_HOLD_KEY) {
	({ offset, sidestepHoldKey }) = deSerialize(blob, offset, "i");
    }
    if (flags & FLAG_SIDESTEP_SPEED) {
	({ offset, sidestepSpeed }) = deSerialize(blob, offset, "f");
    } else {
	sidestepSpeed = 1.0;
    }
    if (flags & FLAG_TURN_COMMAND) {
	({ offset, turnCommand }) = deSerialize(blob, offset, "i");
    }
    if (flags & FLAG_TURN_HOLD_KEY) {
	({ offset, turnHoldKey }) = deSerialize(blob, offset, "i");
    }
    if (flags & FLAG_TURN_SPEED) {
	({ offset, turnSpeed }) = deSerialize(blob, offset, "f");
    } else {
	turnSpeed = 1.0;
    }

    /*
     * motion commands
     */
    n = flags >> 11;
    motionCommands = allocate(n);
    for (i = 0; i < n; i++) {
	motionCommands[i] = motionCommand = allocate(4);
	({
	    offset,
	    motionCommand[0],
	    motionCommand[1],
	    motionCommand[3]
	}) = deSerialize(blob, offset, "ssf");
	motionCommand[2] = motionCommand[1] >> 15;
	motionCommand[1] &= 0x7fff;
    }

    /* position */
    position = new RemotePosition(blob, offset);
    offset += position->size();

    /* sequences + flags */
    ({
	offset,
	instanceSeq,
	controlSeq,
	teleportSeq,
	forcedSeq,
	flags
    }) = deSerialize(blob, offset, "ssssi");
}


int currentHoldKey()		{ return currentHoldKey; }
int currentStance()		{ return currentStance; }
int forwardCommand()		{ return forwardCommand; }
int forwardHoldKey()		{ return forwardHoldKey; }
float forwardSpeed()		{ return forwardSpeed; }
int sidestepCommand()		{ return sidestepCommand; }
int sidestepHoldKey()		{ return sidestepHoldKey; }
float sidestepSpeed()		{ return sidestepSpeed; }
int turnCommand()		{ return turnCommand; }
int turnHoldKey()		{ return turnHoldKey; }
float turnSpeed()		{ return turnSpeed; }
mixed **motionCommands()	{ return motionCommands; }
Position position()		{ return position; }
int instanceSeq()		{ return instanceSeq; }
int controlSeq()		{ return controlSeq; }
int teleportSeq()		{ return teleportSeq; }
int forcedSeq()			{ return forcedSeq; }
int flags()			{ return flags; }
