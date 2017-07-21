# include "Account.h"
# include "Interface.h"


mapping accounts;	/* two-level mapping */

/*
 * initialize account manager
 */
static void create()
{
    accounts = ([ ]);
    compile_object(OBJECT_PATH(Account));
}

/*
 * login
 */
Account login(Interface interface, string name, string password)
{
    if (previous_program() == OBJECT_PATH(Interface)) {
	mapping map;
	Account account;

	/* XXX check blacklist */
	/* XXX check user limit */
	/* XXX check account restrictions */

	map = accounts[name[.. 1]];
	if (map) {
	    account = map[name];
	    if (account) {
		/*
		 * login to known account
		 */
		if (!account->login(interface, password)) {
		    return nil;	/* bad password */
		}

		return account;
	    }
	} else {
	    accounts[name[.. 1]] = map = ([ ]);
	}

	/*
	 * create new account with given name and password
	 */
	return map[name] = clone_object(OBJECT_PATH(Account), name, password);
    }
}

/*
 * logout
 */
void logout(Account account)
{
    if (previous_program() == OBJECT_PATH(Interface)) {
	account->logout();
    }
}


/*
 * find account by name
 */
Account account(string name)
{
    mapping map;

    map = accounts[name[.. 1]];
    return (map) ? map[name] : nil;
}
