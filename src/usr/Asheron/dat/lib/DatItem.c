/*
 * DatItem, a pure data container
 */

# include <String.h>

private int flags;		/* bit flags */
private int id;			/* ID, used for sorting in Btree */
private int timeStamp;		/* creation time */
private int iteration;		/* item iteration */
private StringBuffer data;	/* binary data */

static void create(int flags, int id, int timeStamp, int iteration,
		   StringBuffer data)
{
    ::flags = flags;
    ::id = id;
    ::timeStamp = timeStamp;
    ::iteration = iteration;
    ::data = data;
}

int flags()		{ return flags; }
int id()		{ return id; }
int timeStamp()		{ return timeStamp; }
int iteration()		{ return iteration; }
StringBuffer data()	{ return data; }
