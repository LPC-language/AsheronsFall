# include "Message.h"
# include "Position.h"
# include "Vector.h"

inherit Message;


# define FLAG_VELOCITY		0x01
# define FLAG_PLACEMENT		0x02
# define FLAG_GROUNDED		0x04
# define FLAG_ZERO_QW		0x08
# define FLAG_ZERO_QX		0x10
# define FLAG_ZERO_QY		0x20
# define FLAG_ZERO_QZ		0x40

private int objectId;		/* ID of object */
private int flags;		/* serialization flags */
private Vector velocity;	/* movement velocity */
private int instanceSeq;	/* instance sequence number */
private int positionSeq;	/* position sequence number */
private int teleportSeq;	/* teleport sequence number */
private int forcedSeq;		/* forced position sequence number */
private Position position;	/* position */
private int placement;		/* place in container */

/*
 * message body
 */
static string body()
{
    return serialize("iiifff", objectId, flags, position->cell(), position->X(),
		     position->Y(), position->Z()) +
	   ((flags & FLAG_ZERO_QW) ? "" : serialize("f", position->qW())) +
	   ((flags & FLAG_ZERO_QX) ? "" : serialize("f", position->qX())) +
	   ((flags & FLAG_ZERO_QY) ? "" : serialize("f", position->qY())) +
	   ((flags & FLAG_ZERO_QZ) ? "" : serialize("f", position->qZ())) +
	   ((flags & FLAG_VELOCITY) ? velocity->transport() : "") +
	   ((flags & FLAG_PLACEMENT) ? serialize("i", placement) : "") +
	   serialize("ssss", instanceSeq, positionSeq, teleportSeq, forcedSeq);
}

/*
 * create PositionUpdate
 */
static void create(int objectId, Position position, int grounded,
		   int instanceSeq, int positionSeq, int teleportSeq,
		   int forcedSeq, varargs mixed placement, Vector velocity)
{
    ::create(MSG_UPDATE_POSITION);

    ::objectId = objectId;
    ::position = position;
    flags = (grounded) ? FLAG_GROUNDED : 0;
    ::instanceSeq = instanceSeq;
    ::positionSeq = positionSeq;
    ::teleportSeq = teleportSeq;
    ::forcedSeq = forcedSeq;
    if (placement != nil) {
	::placement = placement;
	flags |= FLAG_PLACEMENT;
    }
    if (velocity != nil) {
	::velocity = velocity;
	flags |= FLAG_VELOCITY;
    }
    if (position->qW() == 0.0) {
	flags |= FLAG_ZERO_QW;
    }
    if (position->qX() == 0.0) {
	flags |= FLAG_ZERO_QX;
    }
    if (position->qY() == 0.0) {
	flags |= FLAG_ZERO_QY;
    }
    if (position->qZ() == 0.0) {
	flags |= FLAG_ZERO_QZ;
    }
}


int objectId()		{ return objectId; }
Position position()	{ return position; }
int grounded()		{ return !!(flags & FLAG_GROUNDED); }
int instancSeq()	{ return instanceSeq; }
int positionSeq()	{ return positionSeq; }
int teleportSeq()	{ return teleportSeq; }
int forcedSeq()		{ return forcedSeq; }
mixed placement()	{ return (flags & FLAG_PLACEMENT) ? placement : nil; }
Vector velocity()	{ return velocity; }
