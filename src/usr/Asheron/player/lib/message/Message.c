# include <String.h>
# include "Serialized.h"
# include "Packet.h"
# include "Message.h"

inherit NetworkData;
inherit Serialized;

static string body()	{ return ""; }
String englishText()	{ return nil; }


private int type;	/* message type */
private int group;	/* message group */

/*
 * layout of header
 */
static string headerLayout()
{
    return "i";
}

/*
 * size (not used)
 */
int size()
{
    return 0;
}

/*
 * message blob to transport across network
 */
string transport()
{
    return serialize(headerLayout(), type) + body();
}

/*
 * initialize message
 */
static void create(int type, int group)
{
    ::create(ND_MESSAGE);
    ::type = type;
    ::group = group;
}


int type()	{ return type; }
int group()	{ return group; }
