# include "Message.h"

inherit Message;


private int error;	/* the actual error */

/*
 * export as a blob
 */
static string body()
{
    return serialize("i", error);
}

/*
 * create CharacterError
 */
static void create(int error)
{
    ::create(MSG_CHARACTER_ERROR, 9);
    ::error = error;
}


int error()	{ return error; }
