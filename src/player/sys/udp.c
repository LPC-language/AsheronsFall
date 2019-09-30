# include <kernel/user.h>
# include "Packet.h"
# include "Interface.h"


# define SYS_INITD	"/usr/System/initd"

object userd;		/* kernel user daemon */

/*
 * initialize UDP connection manager
 */
static void create()
{
    userd = find_object(USERD);
    compile_object(OBJECT_PATH(UDPInterface));
    compile_object(OBJECT_PATH(RemotePacket));
    compile_object(OBJECT_PATH(RemoteFragment));
    compile_object(OBJECT_PATH(RemoteRequestRetransmit));
    compile_object(OBJECT_PATH(RemoteRejectRetransmit));
    compile_object(OBJECT_PATH(RemoteAckSequence));
    compile_object(OBJECT_PATH(RemoteLoginRequest));
    compile_object(OBJECT_PATH(ConnectRequest));
    compile_object(OBJECT_PATH(RemoteConnectError));
    compile_object(OBJECT_PATH(RemoteConnectClose));
    compile_object(OBJECT_PATH(RemoteCICMDCommand));
    compile_object(OBJECT_PATH(RemoteTimeSynch));
    compile_object(OBJECT_PATH(RemoteEchoRequest));
    compile_object(OBJECT_PATH(EchoResponse));
    compile_object(OBJECT_PATH(RemoteFlow));

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
 * let the interface handle login timeouts
 */
int query_timeout(object obj)
{
    return 0;
}
