# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private int serverId;	/* server ID */
private int something;	/* unknown */

/*
 * layout of ServerSwitch
 */
static string headerLayout()
{
    return "ii";
}

/*
 * sizeof the entire blob
 */
int size()
{
    return 8;
}

/*
 * export as a blob
 */
string transport()
{
    return serialize(headerLayout(), serverId, something);
}

/*
 * create a ServerSwitch
 */
static void create(int serverId, int something)
{
    ::serverId = serverId;
    ::something = something;
}


int serverId()	{ return serverId; }
int something()	{ return something; }
