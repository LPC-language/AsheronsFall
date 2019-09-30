# include "Message.h"

inherit Message;


private int playerId;		/* player ID */
private string accountName;	/* account name */

static void create(string blob)
{
    int offset;

    ::create(MSG_CHARACTER_ENTER_WORLD);
    ({ offset, playerId, accountName }) = deSerialize(blob, 4, "it");
}


int playerId()		{ return playerId; }
string accountName()	{ return accountName; }
