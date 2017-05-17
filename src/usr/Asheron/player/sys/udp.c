# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"

# define SYS_INITD	"/usr/System/initd"

object userd;		/* kernel user daemon */

/*
 * initialize UDP connection manager
 */
static void create()
{
    userd = find_object(USERD);
    compile_object(OBJECT_PATH(UDPInterface));
    compile_object(OBJECT_PATH(ClientPacket));
    compile_object(OBJECT_PATH(ClientFragment));
    compile_object(OBJECT_PATH(ClientRequestRetransmit));
    compile_object(OBJECT_PATH(ClientRejectRetransmit));
    compile_object(OBJECT_PATH(ClientAckSequence));
    compile_object(OBJECT_PATH(ClientLoginRequest));
    compile_object(OBJECT_PATH(ConnectRequest));
    compile_object(OBJECT_PATH(ClientCICMDCommand));
    compile_object(OBJECT_PATH(ClientTimeSynch));
    compile_object(OBJECT_PATH(ClientEchoRequest));
    compile_object(OBJECT_PATH(EchoResponse));
    compile_object(OBJECT_PATH(ClientFlow));

    SYS_INITD->set_connection_manager("datagram", 0, this_object());
}

/*
 * select the same interface for all logins
 */
object select(string login)
{
    if (previous_object() == userd) {
	return clone_object(OBJECT_PATH(UDPInterface));
    }
}

/*
 * let the interface handle timeouts
 */
int query_timeout(object obj)
{
    return 0;
}
