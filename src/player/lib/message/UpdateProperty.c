# include "Message.h"

inherit Message;


private int sequence;	/* sequence number */
private string body;	/* serialized property */

/*
 * export as a blob
 */
static string body()
{
    return serialize("c", sequence) + body;
}

/*
 * create UpdateProperty
 */
static void create(int type, string body)
{
    ::create(type);
    ::body = body;
}


setSequence(int sequence)	{ ::sequence = sequence; }

int sequence()			{ return sequence; }
