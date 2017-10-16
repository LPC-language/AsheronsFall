# include <status.h>

/*
 * a client-visible object, not necessarily a physical object
 */


private int class;	/* "weenie" class id */

/*
 * create object
 */
static void create(int class)
{
    ::class = class;
}


int class()	{ return class; }
