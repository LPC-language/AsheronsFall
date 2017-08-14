# include "Interface.h"
# include "Account.h"


string name;		/* account name */
string password;	/* account password */
Interface interface;	/* interface, if logged in */

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
 * login: connect an interface to this account
 */
string login(Interface interface, string str)
{
    if (previous_program() == ACCOUNT_SERVER && !::interface &&
	hash_string("MD5", password[ .. 3] + str) == password[4 ..]) {
	::interface = interface;
	return nil;
    }
    return "";	/* XXX CharacterError message blob */
}

/*
 * logout: disconnect the interface
 */
void logout()
{
    if (previous_program() == ACCOUNT_SERVER) {
	interface = nil;
    }
}


/*
 * fields
 */
string name()		{ return name; }
Interface interface()	{ return interface; }
