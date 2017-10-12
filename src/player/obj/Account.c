# include "Interface.h"
# include "User.h"


# define CHAR_LIST_SIZE	11

string name;		/* account name */
string password;	/* account password */
Character *characters;	/* list of characters */
User user;		/* user, if logged in */

/*
 * Create an account.  Store the password MD5-hashed with a 4-byte random
 * salt.
 */
static void create(string name, string password)
{
    int rand;
    string str;

    characters = ({
	clone_object(OBJECT_PATH(Character), this_object(), "Dworkin"),
	clone_object(OBJECT_PATH(Character), this_object(), "Sunny")
    });

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
	    return ({ nil, LOGGEDON_ALREADY });
	}
	if (hash_string("MD5", password[ .. 3] + str) != password[4 ..]) {
	    return ({ nil, INVALID_ACCOUNT });
	}

	user = clone_object(OBJECT_PATH(User), this_object(), interface);
	return ({ user, nil });
    }
}


/*
 * fields
 */
string name()		{ return name; }
Character *characters()	{ return characters[..]; }
int slots()		{ return CHAR_LIST_SIZE; }
User user()		{ return user; }
