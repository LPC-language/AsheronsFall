# include "Message.h"
# include "User.h"

inherit Message;


private Character *characters;	/* list of characters */
private int slots;		/* maxixmum number of characters */
private string accountName;	/* account name */

/*
 * export the body as a blob
 */
static string body()
{
    int sz, i;
    string body;

    sz = sizeof(characters);
    body = "\0\0\0\0" +
	   serialize("i", sz);
    for (i = 0; i < sz; i++) {
	body += serialize("iti", characters[i]->id(), characters[i]->name(),
			  characters[i]->deleteTimer());
    }
    body += "\0\0\0\0" +
	    serialize("it", slots, accountName) +
	    "\1\0\0\0" +	/* Turbine Chat */
	    "\1\0\0\0";		/* Throne of Destiny */
    return body;
}

/*
 * create a CharacterList message
 */
static void create(Account account)
{
    ::create(MSG_CHARACTER_LIST, 9);

    characters = account->characters();
    slots = account->slots();
    accountName = account->name();
}


Character *characters()	{ return characters; }
int slots()		{ return slots; }
string accountName()	{ return accountName; }
