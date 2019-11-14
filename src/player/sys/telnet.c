# include <kernel/user.h>

private inherit "/lib/util/string";


# define SYS_INITD		"/usr/System/initd"
# define TELNET_INTERFACE	"/usr/Asheron/player/obj/TelnetInterface"

object userd;		/* kernel user daemon */

/*
 * initialize telnet connection manager
 */
static void create()
{
    userd = find_object(USERD);
    compile_object(TELNET_INTERFACE);
    SYS_INITD->set_connection_manager("telnet", 1, this_object());
}

/*
 * select telnet interface for all valid logins
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
 * the initial mode for a telnet login
 */
int query_mode(object obj)
{
    return MODE_ECHO;
}

/*
 * the default timeout for a telnet login
 */
int query_timeout(object obj)
{
    return 30;
}

/*
 * the login banner for a telnet connection
 */
string query_banner(object obj)
{
    return "\nWelcome to Asheron's Call.\n\nlogin: ";
}

/*
 * fallback for invalid login names
 */
int login(string str)
{
    previous_object()->message("Invalid username.\n\n");
    return MODE_DISCONNECT;
}
