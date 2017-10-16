# include "Message.h"

inherit Message;


private int id;		/* id of character to restore */

/*
 * initialize CharacterRestore
 */
static void create(string blob)
{
    int something1, something2;

    ::create(MSG_CHARACTER_RESTORE);
    ({
	blob,
	id,
	something1,
	something2
    }) = deSerialize(blob, "iii");
    checkEmpty(blob);
}


int id()	{ return id; }
