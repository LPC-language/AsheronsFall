# include "Message.h"
# include "User.h"

inherit Message;


private int response;	/* create/restore response */
private int id;		/* ID of created/restored character */
private string name;	/* name of the character */
private int delay;	/* delay before character can login */

/*
 * export as a blob
 */
string body()
{
    if (response == CHARGEN_RESPONSE_OK) {
	return serialize("iiti", response, id, name, delay);
    } else {
	return serialize("i", response);
    }
}

/*
 * create CharacterGenerationResponse message
 */
static void create(int response, varargs int id, string name, int delay)
{
    ::create(MSG_CHARACTER_CREATE_RESPONSE, 9);
    ::response = response;
    ::id = id;
    ::name = name;
    ::delay = delay;
}
