# include "Message.h"

inherit Message;


private string body;	/* serialized property */

/*
 * export as a blob
 */
static string body()
{
    return body;
}

/*
 * create UpdateProperty
 */
static void create(int type, string body)
{
    ::create(type);
    ::body = body;
}
