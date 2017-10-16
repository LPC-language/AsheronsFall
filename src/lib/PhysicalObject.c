# include "Object.h"

inherit Object;

/*
 * an object with a name, dimensions, position, orientation, weight, value
 */


private string name;	/* name of this object */

/*
 * initialize physical object
 */
static void create(int class, string name)
{
    ::create(class);
    ::name = name;
}


string name()	{ return name; }
