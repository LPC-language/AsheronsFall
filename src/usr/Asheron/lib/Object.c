# include <status.h>

/*
 * a client-visible object, not necessarily a physical object
 */


private int id;		/* ID which is unique for object lifetime */
private int type;	/* "weenie" type */

/*
 * create an unique ID for a persistent object
 */
static int persistentId()
{
    int id;

    if (sscanf(object_name(this_object()), "%*s#%d", id) != 0) {
	if (id < 0) {
	    error("Not a persistent object");
	} else {
	    return id;
	}
    } else {
	return status(this_object(), O_INDEX);
    }
}

/*
 * create object
 */
static void create(int id, int type)
{
    ::id = id;
    ::type = type;
}


int id()	{ return id; }
int type()	{ return type; }
