# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private int objectId;	/* object ID */
private int property;	/* the instance property */
private int value;	/* the property value */

/*
 * export as a blob
 */
static string body()
{
    return serialize("ciii", sequence, objectId, property, value);
}

/*
 * create UpdateInstance
 */
static void create(int objectId, int property, int value)
{
    ::create(MSG_UPDATE_INT);
    ::objectId = objectId;
    ::property = property;
    ::value = value;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
int objectId()			{ return objectId; }
int property()			{ return property; }
int value()			{ return value; }
