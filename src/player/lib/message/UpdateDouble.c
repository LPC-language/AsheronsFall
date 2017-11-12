# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private int objectId;	/* object ID */
private int property;	/* the double property */
private float value;	/* the property value */

/*
 * export as a blob
 */
static string body()
{
    return serialize("ciid", sequence, objectId, property, value);
}

/*
 * create UpdateDouble
 */
static void create(int objectId, int property, float value)
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
float value()			{ return value; }
