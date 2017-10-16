# include "Message.h"

inherit Message;


private string accountName;	/* account for which to delete character */
private int slot;		/* character slot in list */

/*
 * initialize CharacterDelete
 */
static void create(string blob)
{
    ::create(MSG_CHARACTER_DELETE);
    ({
	blob,
	accountName,
	slot
    }) = deSerialize(blob, "ti");
    checkEmpty(blob);
}


string accountName()	{ return accountName; }
int slot()		{ return slot; }
