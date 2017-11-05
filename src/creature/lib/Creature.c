# include <status.h>
# include "Object.h"
# include "Creature.h"

inherit PhysicalObject;

/*
 * a creature with vitals, attributes, skills, level, experience and inventory
 */

private int *vitals;
private int *attributes;
private int *skills;
private int level;
private float experience;
private Container *inventory;

static void create(int class, string name, int *attributes, int *skills)
{
    int id;

    if (sscanf(object_name(this_object()), "%*s#%d", id) != 0) {
	if (id < 0) {
	    error("Creature must be a persistent object");
	}
    } else {
	id = status(this_object(), O_INDEX);
    }
    ::create(class, (id << OBJECT_ID_PERSISTENT_SHIFT) +
		    OBJECT_ID_TYPE_PERSISTENT,
	     name);

    ::attributes = attributes;
    ::skills = skills;
    inventory = ({ new Container(102) });
}
