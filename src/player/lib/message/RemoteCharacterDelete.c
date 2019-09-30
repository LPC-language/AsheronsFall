# include "Message.h"

inherit Message;


private string accountName;	/* account for which to delete character */
private int slot;		/* character slot in list */

/*
 * initialize CharacterDelete
 */
static void create(string blob)
{
    int offset;

    ::create(MSG_CHARACTER_DELETE);
    ({
	offset,
	accountName,
	slot
    }) = deSerialize(blob, 4, "ti");
    checkEmpty(blob, offset);
}


string accountName()	{ return accountName; }
int slot()		{ return slot; }
