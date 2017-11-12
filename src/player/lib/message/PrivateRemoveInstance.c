# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private int property;	/* the instance property */

/*
 * export as a blob
 */
static string body()
{
    return serialize("ci", sequence, property);
}

/*
 * create PrivateRemoveInsgtance
 */
static void create(int property)
{
    ::create(MSG_PRIVATE_REMOVE_INSTANCE);
    ::property = property;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
int property()			{ return property; }
