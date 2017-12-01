# include "Interface.h"
# include "User.h"


# define CHARACTERS_MAX		11

string name;			/* account name */
string password;		/* account password */
Character *characters;		/* list of characters */
User user;			/* user, if logged in */

/*
 * Create an account.  Store the password MD5-hashed with a 4-byte random
 * salt.
 */
static void create(string name, string password)
{
    int rand;
    string str;

    characters = ({ });

    ::name = name;
    rand = random(0);
    str = "\0\0\0\0";
    str[0] = rand;
    str[1] = rand >> 8;
    str[2] = rand >> 16;
    str[3] = rand >> 24;
    ::password = str + hash_string("MD5", str + password);
}

/*
 * login on this account
 */
mixed *login(Interface interface, string str)
{
    if (previous_program() == ACCOUNT_SERVER) {
	if (user) {
	    return ({ nil, CHARERR_LOGGEDON_ALREADY });
	}
	if (hash_string("MD5", password[ .. 3] + str) != password[4 ..]) {
	    return ({ nil, CHARERR_INVALID_ACCOUNT });
	}

	user = clone_object(OBJECT_PATH(User), this_object(), interface);
	return ({ user, nil });
    }
}

/*
 * create a new character for this account
 */
atomic int *characterCreate(string name)
{
    if (previous_object() == user) {
	object users;
	Character player;

	if (sizeof(characters) == CHARACTERS_MAX) {
	    return ({ CHARGEN_RESPONSE_CORRUPT, 0 });
	}
	users = find_object(USER_SERVER);
	if (users->character(name)) {
	    return ({ CHARGEN_RESPONSE_NAME_IN_USE, 0 });
	}
	if (users->characterBanned(name)) {
	    return ({ CHARGEN_RESPONSE_NAME_BANNED, 0 });
	}

	player = clone_object(OBJECT_PATH(Character), this_object(), name);
	characters += ({ player });
	users->characterAdd(player);
	return ({ CHARGEN_RESPONSE_OK, player->id() });
    }
}

/*
 * find a character by ID
 */
Character character(int id)
{
    int sz, i;
    Character player;

    for (sz = sizeof(characters), i = 0; i < sz; i++) {
	player = characters[i];
	if (player->id() == id) {
	    return player;
	}
    }
    return nil;
}

/*
 * delete a character for this account, with a timeout
 */
atomic int characterDelete(int slot)
{
    if (previous_object() == user && slot >= 0 && slot < sizeof(characters)) {
	Character player;

	player = characters[slot];
	if (player->timedDelete()) {
	    USER_SERVER->characterRemove(player);
	    return CHARERR_OK;
	}
    }
    return CHARERR_DELETE;
}

/*
 * restore a deleted character, if possible
 */
atomic mixed *characterRestore(int id)
{
    if (previous_object() == user) {
	Character player;
	object users;

	player = character(id);
	if (player && player->deleteTimer() != 0) {
	    users = find_object(USER_SERVER);
	    if (users->character(player->name())) {
		return ({ CHARGEN_RESPONSE_NAME_IN_USE, nil });
	    }
	    if (users->characterBanned(player->name())) {
		return ({ CHARGEN_RESPONSE_NAME_BANNED, nil });
	    }

	    player->cancelDelete();
	    users->characterAdd(player);
	    return ({ CHARGEN_RESPONSE_OK, player });
	}
	return ({ CHARGEN_RESPONSE_CORRUPT, nil });
    }
}

/*
 * clean up and return the list of characters
 */
Character *characters()
{
    characters -= ({ nil });
    return characters[..];
}


/*
 * fields
 */
string name()		{ return name; }
int slots()		{ return CHARACTERS_MAX; }
User user()		{ return user; }
