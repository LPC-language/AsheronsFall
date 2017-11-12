# include <Time.h>
# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private int objectId;	/* object ID */
private int property;	/* the Time property */
private Time value;	/* the property value */

/*
 * export as a blob
 */
static string body()
{
    return serialize("ciiD", sequence, objectId, property, value);
}

/*
 * create UpdateTime
 */
static void create(int objectId, int property, Time value)
{
    ::create(MSG_UPDATE_DOUBLE);
    ::objectId = objectId;
    ::property = property;
    ::value = value;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
int objectId()			{ return objectId; }
int property()			{ return property; }
Time value()			{ return value; }
