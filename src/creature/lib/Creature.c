# include <status.h>
# include "Object.h"
# include "Creature.h"
# include "Properties.h"

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

static int getIntProperty(int prop)
{
    switch (prop) {
    case PROP_INT_LEVEL:
    case PROP_INT_GENDER:
	return 1;

    case PROP_INT_ENCUMB_VAL:
	return 3943;

    case PROP_INT_WEAPON_AURA_DAMAGE:
    case PROP_INT_WEAPON_AURA_SPEED:
    case PROP_INT_DOT_RESIST_RATING:
    case PROP_INT_LIFE_RESIST_RATING:
    case PROP_INT_DAMAGE_RATING:
    case PROP_INT_DAMAGE_RESIST_RATING:
    case PROP_INT_CRIT_RATING:
    case PROP_INT_CRIT_DAMAGE_RATING:
    case PROP_INT_CRIT_RESIST_RATING:
    case PROP_INT_CRIT_DAMAGE_RESIST_RATING:
    case PROP_INT_WEAKNESS_RATING:
    case PROP_INT_NETHER_OVER_TIME:
    case PROP_INT_NETHER_RESIST_RATING:
    case PROP_INT_HEALING_RESIST_RATING:
    case PROP_INT_HEALING_BOOST_RATING:
    case PROP_INT_DAMAGE_OVER_TIME:
    case PROP_INT_HEAL_OVER_TIME:
    case PROP_INT_GEAR_DAMAGE:
    case PROP_INT_GEAR_DAMAGE_RESIST:
    case PROP_INT_GEAR_CRIT:
    case PROP_INT_GEAR_CRIT_RESIST:
    case PROP_INT_GEAR_CRIT_DAMAGE:
    case PROP_INT_GEAR_CRIT_DAMAGE_RESIST:
    case PROP_INT_GEAR_HEALING_BOOST:
    case PROP_INT_GEAR_NETHER_RESIST:
    case PROP_INT_GEAR_LIFE_RESIST:
    case PROP_INT_GEAR_MAX_HEALTH:
    case PROP_INT_GEAR_PK_DAMAGE_RATING:
    case PROP_INT_GEAR_PK_DAMAGE_RESIST_RATING:
    case PROP_INT_OVERPOWER:
    case PROP_INT_OVERPOWER_RESIST:
    case PROP_INT_GEAR_OVERPOWER:
    case PROP_INT_GEAR_OVERPOWER_RESIST:
	return 0;

    default:
	return ::getIntProperty(prop);
    }
}

static float getDoubleProperty(int prop)
{
    switch (prop) {
    case PROP_DOUBLE_WEAPON_AURA_OFFENSE:
    case PROP_DOUBLE_WEAPON_AURA_DEFENSE:
    case PROP_DOUBLE_WEAPON_AURA_ELEMENTAL:
	return 0.0;

    case PROP_DOUBLE_WEAPON_AURA_MANA_CONV:
    case PROP_DOUBLE_RESIST_HEALTH_DRAIN:
	return 1.0;

    default:
	return ::getDoubleProperty(prop);
    }
}

static string getStringProperty(int prop)
{
    if (prop == PROP_STRING_NAME) {
	return name();
    }

    return ::getStringProperty(prop);
}

static int getDataProperty(int prop)
{
    switch (prop) {
    case PROP_DATA_MOTION_TABLE:
	return 150994945;

    case PROP_DATA_COMBAT_TABLE:
	return 805306368;

    default:
	return ::getDataProperty(prop);
    }
}
