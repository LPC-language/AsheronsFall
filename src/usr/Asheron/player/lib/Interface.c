# include "Account.h"

inherit "~System/lib/user";


/*
 * login through the account server
 */
static Account login(string name, string password)
{
    Account account;

    if (query_conn()) {
	account = ACCOUNT_SERVER->login(this_object(), name, password);
	if (account) {
	    ::login("Connection: " + name + " from " + address() + "\n");
	}
    }
    return account;
}

/*
 * logout through the account server
 */
static void logout(Account account)
{
    if (query_conn()) {
	ACCOUNT_SERVER->logout(account);
    }
}
