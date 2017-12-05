# include <status.h>
# include <Time.h>
# include "Position.h"
# include "Serialized.h"
# include "properties.h"

/*
 * a client-visible object, not necessarily a physical object
 */

inherit Serialized;


private int class;	/* "weenie" class id */
private int id;		/* unique ID that identifies this object */

/*
 * create object
 */
static void create(int class, int id)
{
    ::class = class;
    ::id = id;
}


static void setId(int id)	{ ::id = id; }

int class()			{ return class; }
int id()			{ return id; }


/* ========================================================================= *
 *				properties				     *
 * ========================================================================= */

/*
 * bool properties
 */

static int boolProperty(int prop)
{
    error("Unknown bool property " + prop);
}

string getBoolProperty(int prop)
{
    return serialize("ii", prop, boolProperty(prop));
}

/*
 * int properties
 */

static int intProperty(int prop)
{
    error("Unknown int property " + prop);
}

string getIntProperty(int prop)
{
    return serialize("ii", prop, intProperty(prop));
}

/*
 * long properties
 */

static float longProperty(int prop)
{
    error("Unknown long property " + prop);
}

string getLongProperty(int prop)
{
    return serialize("il", prop, longProperty(prop));
}

/*
 * double properties
 */

static float doubleProperty(int prop)
{
    error("Unknown double property " + prop);
}

static Time timeProperty(int prop)
{
    error("Unknown time property " + prop);
}

string getDoubleProperty(int prop)
{
    switch (prop) {
    case PROP_DOUBLE_HEARTBEAT_TIMESTAMP:
    case PROP_DOUBLE_MOTION_TIMESTAMP:
    case PROP_DOUBLE_REGENERATION_TIMESTAMP:
    case PROP_DOUBLE_DEATH_TIMESTAMP:
    case PROP_DOUBLE_PK_TIMESTAMP:
    case PROP_DOUBLE_VICTIM_TIMESTAMP:
    case PROP_DOUBLE_LOGIN_TIMESTAMP:
    case PROP_DOUBLE_CREATION_TIMESTAMP:
    case PROP_DOUBLE_ABUSE_LOGGING_TIMESTAMP:
    case PROP_DOUBLE_LAST_PORTAL_TELEPORT_TIMESTAMP:
    case PROP_DOUBLE_RELEASED_TIMESTAMP:
    case PROP_DOUBLE_RESET_TIMESTAMP:
    case PROP_DOUBLE_LOGOFF_TIMESTAMP:
    case PROP_DOUBLE_ITEM_MANA_UPDATE_TIMESTAMP:
    case PROP_DOUBLE_ALLEGIANCE_APPRAISAL_TIMESTAMP:
    case PROP_DOUBLE_ATTACK_TIMESTAMP:
    case PROP_DOUBLE_CHECKPOINT_TIMESTAMP:
    case PROP_DOUBLE_SOLD_TIMESTAMP:
    case PROP_DOUBLE_USE_TIMESTAMP:
    case PROP_DOUBLE_USE_LOCK_TIMESTAMP:
    case PROP_DOUBLE_FROZEN_TIMESTAMP:
    case PROP_DOUBLE_ALLEGIANCE_SWEAR_TIMESTAMP:
    case PROP_DOUBLE_SPAM_TIMESTAMP:
    case PROP_DOUBLE_GAG_TIMESTAMP:
    case PROP_DOUBLE_GENERATOR_UPDATE_TIMESTAMP:
    case PROP_DOUBLE_DEATH_SPAM_TIMESTAMP:
    case PROP_DOUBLE_LIFESTONE_PROTECTION_TIMESTAMP:
    case PROP_DOUBLE_TRADE_TIMESTAMP:
    case PROP_DOUBLE_LAST_TELEPORT_START_TIMESTAMP:
    case PROP_DOUBLE_EVENT_SPAM_TIMESTAMP:
    case PROP_DOUBLE_ALLEGIANCE_INFO_SPAM_TIMESTAMP:
    case PROP_DOUBLE_NEXT_SPELLCAST_TIMESTAMP:
    case PROP_DOUBLE_APPRAISAL_REQUESTED_TIMESTAMP:
    case PROP_DOUBLE_APPRAISAL_HEARTBEAT_DUE_TIMESTAMP:
    case PROP_DOUBLE_LAST_PK_ATTACK_TIMESTAMP:
    case PROP_DOUBLE_FELLOWSHIP_UPDATE_TIMESTAMP:
    case PROP_DOUBLE_LIMBO_START_TIMESTAMP:
    case PROP_DOUBLE_START_MISSILE_ATTACK_TIMESTAMP:
    case PROP_DOUBLE_LAST_RARE_USED_TIMESTAMP:
    case PROP_DOUBLE_ALLEGIANCE_GAG_TIMESTAMP:
	return serialize("iD", prop, timeProperty(prop));

    default:
	return serialize("id", prop, doubleProperty(prop));
    }
}

/*
 * string properties
 */

static string stringProperty(int prop)
{
    error("Unknown string property " + prop);
}

string getStringProperty(int prop)
{
    return serialize("it", prop, stringProperty(prop));
}

/*
 * data properties
 */

static int dataProperty(int prop)
{
    error("Unknown data property " + prop);
}

string getDataProperty(int prop)
{
    return serialize("ii", prop, dataProperty(prop));
}

/*
 * instance properties
 */

static int instanceProperty(int prop)
{
    error("Unknown instance property " + prop);
}

string getInstanceProperty(int prop)
{
    return serialize("ii", prop, instanceProperty(prop));
}

/*
 * position properties
 */

static Position positionProperty(int prop)
{
    error("Unknown position property " + prop);
}

string getPositionProperty(int prop)
{
    return serialize("i", prop) + positionProperty(prop)->transport();
}
