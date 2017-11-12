# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private int property;	/* the long property */
private float value;	/* the property value */

/*
 * export as a blob
 */
static string body()
{
    return serialize("cid", sequence, property, value);
}

/*
 * create PrivateUpdateDouble
 */
static void create(int property, float value)
{
    ::create(MSG_PRIVATE_UPDATE_DOUBLE);
    ::property = property;
    ::value = value;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
int property()			{ return property; }
float value()			{ return value; }
