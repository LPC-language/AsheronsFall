# include "Message.h"

inherit Message;

static string eventBody();


private int playerId;		/* player ID */
private int sequence;		/* event sequence number */
private int eventType;		/* event type */

/*
 * export as a blob
 */
static string body()
{
    return serialize("iii", playerId, sequence, eventType) + eventBody();
}

/*
 * create GameEvent message
 */
static void create(int eventType, int playerId)
{
    ::create(MSG_GAME_EVENT);

    ::eventType = eventType;
    ::playerId = playerId;
}


void setSequence(int sequence)	{ ::sequence = sequence; }

int playerId()			{ return playerId; }
int sequence()			{ return sequence; }
int eventType()			{ return eventType; }
