# include "Message.h"

inherit Message;


private string serverName;	/* the name of this server */
private int users;		/* number of logged-in users */
private int maxUsers;		/* maximum number of users */

/*
 * export as a blob
 */
static string body()
{
    return serialize("iit", users, maxUsers, serverName);
}

/*
 * create ServerName message
 */
static void create(string serverName, int users, int maxUsers)
{
    ::create(MSG_SERVER_NAME, 9);

    ::serverName = serverName;
    ::users = users;
    ::maxUsers = maxUsers;
}


string serverName()	{ return serverName; }
int users()		{ return users; }
int maxUsers()		{ return maxUsers; }
