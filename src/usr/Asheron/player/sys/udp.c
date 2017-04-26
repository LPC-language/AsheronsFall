# include <kernel/user.h>

# define SYS_INITD	"/usr/System/initd"
# define UDP_INTERFACE	"/usr/Asheron/player/obj/UDPInterface"

object userd;		/* kernel user daemon */

/*
 * NAME:	create()
 * DESCRIPTION:	initialize this object
 */
static void create()
{
    userd = find_object(USERD);
    compile_object(UDP_INTERFACE);
    SYS_INITD->set_connection_manager("datagram", 0, this_object());
}

/*
 * NAME:	select()
 * DESCRIPTION:	select interface based on login packet
 */
object select(string login)
{
    if (previous_object() == userd) {
	return clone_object(UDP_INTERFACE);
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
