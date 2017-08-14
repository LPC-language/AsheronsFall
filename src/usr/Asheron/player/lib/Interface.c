# include "Account.h"

inherit "~System/lib/user";


/*
 * login through the account server
 */
static mixed *login(string name, string password)
{
    Account account;
    string message;

    if (query_conn()) {
	({ account, message }) =
			ACCOUNT_SERVER->login(this_object(), name, password);
	if (account) {
	    ::login("Connection: " + name + " from " + address() + "\n");
	}
    }
    return ({ account, message });
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
