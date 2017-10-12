# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private string version;		/* client version */
private int size;		/* size of the remaining part */
private int flags;		/* bitflags */
private int time;		/* client time */
private string account;		/* player account */
private string ticket;		/* GLS ticket (password starting with \n) */

/*
 * layout of everything but the ticket
 */
static string headerLayout()
{
    return "tissiitii";
}

/*
 * size of the entire blob
 */
int size()
{
    int len;

    len = 6 + strlen(version);
    len = (len + 3) & ~3;
    return len + size;
}

/*
 * export as a blob
 */
string transport()
{
    int authType;
    string str;

    if (ticket) {
	authType = AUTH_ACCOUNT_TICKET;
	str = ticket;
    } else {
	authType = AUTH_ACCOUNT;
	str = "";
    }
    return serialize(headerLayout(), version, size, authType, flags, 0, time,
		     account, 0, strlen(str)) + str;
}

/*
 * create a LoginRequest with the given fields
 */
static void create(string version, int flags, int time, string account,
		   string ticket)
{
    ::create(PACKET_LOGIN_REQUEST);

    ::version = version;
    ::size = 20 + 2 + strlen(account);
    ::size = (::size + 3) & ~3;
    if (ticket) {
	::size += strlen(ticket);
    }
    ::flags = flags;
    ::time = time;
    ::account = account;
    ::ticket = ticket;
}

/*
 * fields
 */
string version()	{ return version; }
int flags()		{ return flags; }
int time()		{ return time; }
string account()	{ return account; }
string ticket()		{ return ticket; }
