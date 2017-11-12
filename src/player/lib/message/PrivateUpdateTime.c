# include <Time.h>
# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private int property;	/* the Time property */
private Time value;	/* property value */

/*
 * export as a blob
 */
static string body()
{
    return serialize("ciD", sequence, property, value);
}

/*
 * create PrivateUpdateTime
 */
static void create(int property, Time value)
{
    ::create(MSG_PRIVATE_UPDATE_DOUBLE);
    ::property = property;
    ::value = value;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
int property()			{ return property; }
Time value()			{ return value; }
