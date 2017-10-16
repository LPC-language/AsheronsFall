# include "Message.h"

inherit Message;


private string body;	/* body of message */

/*
 * prefab body
 */
static string body()
{
    return body;
}

/*
 * generic message
 */
static void create(int mesg, string body)
{
    ::create(mesg);
    ::body = body;
}
