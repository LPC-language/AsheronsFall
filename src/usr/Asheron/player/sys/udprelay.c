# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"

# define SYS_INITD	"/usr/System/initd"

object userd;		/* kernel user daemon */

/*
 * NAME:	create()
 * DESCRIPTION:	initialize this object
 */
static void create()
{
    userd = find_object(USERD);
    compile_object(OBJECT_PATH(UDPRelay));
    compile_object(OBJECT_PATH(ClientConnectResponse));
    SYS_INITD->set_connection_manager("datagram", 1, this_object());
}

/*
 * NAME:	select()
 * DESCRIPTION:	select interface based on connect packet
 */
object select(string connect)
{
    if (previous_object() == userd) {
	return clone_object(OBJECT_PATH(UDPRelay));
    }
}

/*
 * NAME:	query_timeout()
 * DESCRIPTION:	return the login timeout
 */
int query_timeout(object obj)
{
    return 0;
}
