# include "Message.h"

inherit Message;


private int playerId;		/* id of character to restore */

/*
 * initialize CharacterRestore
 */
static void create(string blob)
{
    int offset, something1, something2;

    ::create(MSG_CHARACTER_RESTORE);
    ({
	offset,
	playerId,
	something1,
	something2
    }) = deSerialize(blob, 4, "iii");
    checkEmpty(blob, offset);
}


int playerId()	{ return playerId; }
