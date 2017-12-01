# include "Message.h"
# include "User.h"

inherit Message;


private Character *characters;	/* list of characters */
private int slots;		/* maxixmum number of characters */
private string accountName;	/* account name */

/*
 * serialize a character
 */
static string player(Character player)
{
    return serialize("iti", player->id(), player->name(),
		     -player->deleteTimer());
}

/*
 * export the body as a blob
 */
static string body()
{
    string body;

    body = "\0\0\0\0" + serializeArray(characters, this_object(), "player");
    body += "\0\0\0\0" +
	    serialize("it", slots, accountName) +
	    "\1\0\0\0" +	/* Turbine Chat */
	    "\1\0\0\0";		/* Throne of Destiny */
    return body;
}

/*
 * create a CharacterList message
 */
static void create(Character *characters, int slots, string accountName)
{
    ::create(MSG_CHARACTER_LIST);

    ::characters = characters;
    ::slots = slots;
    ::accountName = accountName;
}


Character *characters()	{ return characters; }
int slots()		{ return slots; }
string accountName()	{ return accountName; }
