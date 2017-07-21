# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"


# define SYS_INITD	"/usr/System/initd"

object userd;		/* kernel user daemon */

/*
 * initialize UDP relay cnnection manager
 */
static void create()
{
    userd = find_object(USERD);
    compile_object(OBJECT_PATH(UDPRelay));
    compile_object(OBJECT_PATH(ClientConnectResponse));

    SYS_INITD->set_connection_manager("datagram", 1, this_object());
}

/*
 * select UDP relay interface for all connections
 */
object select(string connect)
{
    if (previous_object() == userd) {
	return clone_object(OBJECT_PATH(UDPRelay));
    }
}

/*
 * let the interface handle login timeouts
 */
int query_timeout(object obj)
{
    return 0;
}
