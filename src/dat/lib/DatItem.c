/*
 * DatItem, a pure data container
 */

# include "Serialized.h"


inherit Serialized;

private int flags;		/* bit flags */
private int id;			/* ID, used for sorting in Btree */
private int block;		/* first block of item */
private int length;		/* length of item */
private int timeStamp;		/* creation time */
private int iteration;		/* item iteration */

static void create(string entry)
{
    ({
	flags,
	id,
	block,
	length,
	timeStamp,
	iteration
    }) = deSerialize(entry, "iiiiii")[1 ..];
}

int flags()		{ return flags; }
int id()		{ return id; }
int block()		{ return block; }
int length()		{ return length; }
int timeStamp()		{ return timeStamp; }
int iteration()		{ return iteration; }
