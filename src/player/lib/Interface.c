# include <status.h>
# include <Time.h>
# include "User.h"

inherit "~System/lib/user";


/*
 * time since the server first booted (call with current time)
 */
static Time timeServer(int time, float mtime)
{
    return new Time(time - status(ST_STARTTIME), mtime);
}

/*
 * time difference
 */
static float timeDiff(Time timeA, Time timeB)
{
    return timeB->mtime() - timeA->mtime() +
	   (float) (timeB->time() - timeA->time());
}

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
