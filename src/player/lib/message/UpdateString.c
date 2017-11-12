# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private int objectId;	/* object ID */
private int property;	/* the string property */
private string value;	/* the property value */

/*
 * export as a blob
 */
static string body()
{
    /*
     * note the order: property, objectId, value
     */
    return serializeAlign(serialize("cii", sequence, property, objectId)) +
	   serialize("t", value);
}

/*
 * create UpdateString
 */
static void create(int objectId, int property, string value)
{
    ::create(MSG_UPDATE_STRING);
    ::objectId = objectId;
    ::property = property;
    ::value = value;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
int objectId()			{ return objectId; }
int property()			{ return property; }
string value()			{ return value; }
