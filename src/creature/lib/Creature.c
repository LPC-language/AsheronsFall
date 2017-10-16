# include <status.h>
# include "Object.h"
# include "Creature.h"

inherit PhysicalObject;

/*
 * a creature with vitals, attributes, skills, level, experience and inventory
 */

private int id;
private int health, healthMax;
private int stamina, staminaMax;
private int mana, manaMax;
private int strength;
private int endurance;
private int coordination;
private int quickness;
private int focus;
private int self;
private int *skills;
private int level;
private float experience;
private Container *inventory;

static void create(int class, string name, int *attributes, int *skills)
{
    ::create(class, name);
    if (sscanf(object_name(this_object()), "%*s#%d", id) != 0) {
	if (id < 0) {
	    error("Creature must be a persistent object");
	}
    } else {
	id = status(this_object(), O_INDEX);
    }
    id = (id << 16) + OBJECT_ID_PERSISTENT;

    ({ strength, endurance, coordination, quickness, focus, self }) =
								    attributes;
    ::skills = skills;
    inventory = ({ new Container(102) });
}


int id()	{ return id; }
