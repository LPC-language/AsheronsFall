# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


# define HEADER_SIZE	16

private int sequence;	/* sequence number */
private int id;		/* ID */
private int count;	/* fragments in message */
private int index;	/* index in message */
private int group;	/* group */
private string body;	/* message data */

/*
 * layout of header
 */
static string headerLayout()
{
    return "iissss";
}

/*
 * size including header
 */
int size()
{
    return HEADER_SIZE + strlen(body);
}

/*
 * export as a blob
 */
string transport()
{
    return serialize(headerLayout(), sequence, id, count, size(), index,
		     group) + body;
}

/*
 * initialize
 */
static void create(int sequence, int id, int count, int index, int group,
		   string body)
{
    ::create(PACKET_BLOB_FRAGMENTS);

    ::sequence = sequence;
    ::id = id;
    ::count = count;
    ::index = index;
    ::group = group;
    ::body = body;
}

/*
 * fields
 */
int sequence()	{ return sequence; }
int id()	{ return id; }
int count()	{ return count; }
int index()	{ return index; }
int group()	{ return group; }
string body()	{ return body; }
