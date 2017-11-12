# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private int property;	/* the data property */
private int value;	/* the property value */

/*
 * export as a blob
 */
static string body()
{
    return serialize("cii", sequence, property, value);
}

/*
 * create PrivateUpdateData
 */
static void create(int property, int value)
{
    ::create(MSG_PRIVATE_UPDATE_DATA);
    ::property = property;
    ::value = value;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
int property()			{ return property; }
int value()			{ return value; }
