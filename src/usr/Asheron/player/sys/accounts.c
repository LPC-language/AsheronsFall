# include "Account.h"

inherit "/lib/util/string";


mapping accounts;
mapping loggedIn;

static void create()
{
    accounts = ([ ]);
    loggedIn = ([ ]);
    compile_object(OBJECT_PATH(Account));
}

Account findAccount(string name)
{
    mapping map;

    map = accounts[name[.. 1]];
    return (map) ? map[name] : nil;
}

Account account(string name)
{
    /* XXX ecority check */
    mapping map;
    Account account;

    map = accounts[name[.. 1]];
    if (map) {
	account = map[name];
	if (account) {
	    return account;
	}
    } else {
	accounts[name[.. 1]] = map = ([ ]);
    }

    return map[name] = clone_object(OBJECT_PATH(Account), name);
}

void loginAccount(string name)
{
    if (previous_program() == OBJECT_PATH(Account)) {
	loggedIn[previous_object()] = name;
    }
}

void logoutAccount(string name)
{
    if (previous_program() == OBJECT_PATH(Account)) {
	loggedIn[previous_object()] = nil;
    }
}
