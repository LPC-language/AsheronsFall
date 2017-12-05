# include "Message.h"

inherit Message;


private int playerId;		/* player ID */
private int sequence;		/* event sequence number */
private int eventType;		/* event type */
private string eventBody;	/* event body */

/*
 * export as a blob
 */
static string body()
{
    return serialize("iii", playerId, sequence, eventType) + eventBody;
}

/*
 * create GameEvent message
 */
static void create(int eventType, object player, string eventBody)
{
    ::create(MSG_GAME_EVENT);

    ::eventType = eventType;
    playerId = player->id();
    ::eventBody = eventBody;
}


void setSequence(int sequence)	{ ::sequence = sequence; }

int playerId()			{ return playerId; }
int sequence()			{ return sequence; }
int eventType()			{ return eventType; }
