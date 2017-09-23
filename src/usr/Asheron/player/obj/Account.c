# include "User.h"
# include "Interface.h"


string name;		/* account name */
string password;	/* account password */
User user;		/* user, if logged in */

/*
 * Create an account.  Store the password MD5-hashed with a 4-byte random
 * salt.
 */
static void create(string name, string password)
{
    int rand;
    string str;

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
    if (previous_program() == ACCOUNT_SERVER && !user &&
	hash_string("MD5", password[ .. 3] + str) == password[4 ..]) {
	user = clone_object(OBJECT_PATH(User), this_object(), interface);
	return ({ user, nil });
    }
    return ({ nil, "\x59\xf6\x00\x00\x09\x00\x00\x00" }); /* invalid account */
}


/*
 * fields
 */
string name()	{ return name; }
User user()	{ return user; }
