# include "Message.h"
# include "Position.h"

inherit Message;


private int sequence;	/* sequence number */
private int property;	/* the Position property */
private Position value;	/* property value */

/*
 * export as a blob
 */
static string body()
{
    return serialize("ci", sequence, property) + value->transport();
}

/*
 * create PrivateUpdatePosition
 */
static void create(int property, Position value)
{
    ::create(MSG_PRIVATE_UPDATE_DOUBLE);
    ::property = property;
    ::value = value;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
int property()			{ return property; }
Position value()		{ return value; }
