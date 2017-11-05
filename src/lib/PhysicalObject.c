# include "Object.h"

inherit Object;

/*
 * an object with a name, dimensions, position, orientation, weight
 */


private string name;	/* name of this object */

/*
 * initialize physical object
 */
static void create(int class, int id, string name)
{
    ::create(class, id);
    ::name = name;
}


string name()	{ return name; }
