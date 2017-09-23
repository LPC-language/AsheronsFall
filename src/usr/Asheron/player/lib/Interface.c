# include "User.h"

inherit "~System/lib/user";


/*
 * login through the account server
 */
static mixed *login(string name, string password)
{
    User user;
    string message;

    if (query_conn()) {
	({ user, message }) =
			ACCOUNT_SERVER->login(this_object(), name, password);
	if (user) {
	    ::login("Connection: " + name + " from " + address() + "\n");
	}
    }
    return ({ user, message });
}

/*
 * logout the user
 */
static void logout(User user)
{
    if (query_conn()) {
	user->logout();
    }
}
