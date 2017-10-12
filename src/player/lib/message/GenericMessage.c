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
static void create(int type, int group, string body)
{
    ::create(type, group);
    ::body = body;
}
