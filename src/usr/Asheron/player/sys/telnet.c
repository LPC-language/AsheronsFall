# include <kernel/user.h>

private inherit "/lib/util/string";


# define SYS_INITD		"/usr/System/initd"
# define TELNET_INTERFACE	"/usr/Asheron/player/obj/TelnetInterface"

object userd;		/* kernel user daemon */

/*
 * NAME:	create()
 * DESCRIPTION:	initialize this object
 */
static void create()
{
    userd = find_object(USERD);
    compile_object(TELNET_INTERFACE);
    SYS_INITD->set_connection_manager("telnet", 1, this_object());
}

/*
 * NAME:	select()
 * DESCRIPTION:	select interface based on loginname
 */
object select(string name)
{
    if (previous_object() == userd) {
	if (!parse_string("word = /[A-Za-z][A-Za-z]+/ junk = /./ name: word",
			  name)) {
	    /* bad name */
	    return this_object();
	}

	return clone_object(TELNET_INTERFACE);
    }
}

/*
 * NAME:	query_timeout()
 * DESCRIPTION:	return the login timeout
 */
int query_timeout(object obj)
{
    return 30;
}

/*
 * NAME:	query_banner()
 * DESCRIPTION:	return the login banner
 */
string query_banner(object obj)
{
    return "\nWelcome to Asheron's Call.\n\nlogin: ";
}

/*
 * NAME:	login()
 * DESCRIPTION:	display an errormessage and disconnect
 */
int login(string str)
{
    previous_object()->message("Invalid username.\n\n");
    return MODE_DISCONNECT;
}
