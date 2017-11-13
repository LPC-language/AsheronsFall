# include <status.h>
# include <Time.h>
# include "Position.h"
# include "Serialized.h"
# include "Properties.h"

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

static int getBoolProperty(int prop)
{
    error("Unknown bool property " + prop);
}

string getBoolProperties(int *props)
{
    int sz, i, *values;
    string *format;

    sz = sizeof(props);
    format = allocate(sz);
    values = allocate_int(sz * 2);
    for (i = 0; i < sz; i++) {
	format[i] = "ii";
	values[i * 2] = props[i];
	values[i * 2 + 1] = getBoolProperty(props[i]);
    }
    return serialize(implode(format, ""), values...);
}

/*
 * int properties
 */

static int getIntProperty(int prop)
{
    error("Unknown int property " + prop);
}

string getIntProperties(int *props)
{
    int sz, i, *values;
    string *format;

    sz = sizeof(props);
    format = allocate(sz);
    values = allocate_int(sz * 2);
    for (i = 0; i < sz; i++) {
	format[i] = "ii";
	values[i * 2] = props[i];
	values[i * 2 + 1] = getIntProperty(props[i]);
    }
    return serialize(implode(format, ""), values...);
}

/*
 * long properties
 */

static float getLongProperty(int prop)
{
    error("Unknown long property " + prop);
}

string getLongProperties(int *props)
{
    int sz, i;
    string *format;
    mixed *values;

    sz = sizeof(props);
    format = allocate(sz);
    values = allocate(sz * 2);
    for (i = 0; i < sz; i++) {
	format[i] = "il";
	values[i * 2] = props[i];
	values[i * 2 + 1] = getLongProperty(props[i]);
    }
    return serialize(implode(format, ""), values...);
}

/*
 * double properties
 */

static float getDoubleProperty(int prop)
{
    error("Unknown double property " + prop);
}

static Time getTimeProperty(int prop)
{
    error("Unknown time property " + prop);
}

private int isTimeProperty(int prop)
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
	return TRUE;

    default:
	return FALSE;
    }
}

string getDoubleProperties(int *props)
{
    int sz, i;
    string *format;
    mixed *values;

    sz = sizeof(props);
    format = allocate(sz);
    values = allocate(sz * 2);
    for (i = 0; i < sz; i++) {
	values[i * 2] = props[i];
	if (isTimeProperty(props[i])) {
	    format[i] = "iD";
	    values[i * 2 + 1] = getTimeProperty(props[i]);
	} else {
	    format[i] = "id";
	    values[i * 2 + 1] = getDoubleProperty(props[i]);
	}
    }
    return serialize(implode(format, ""), values...);
}

/*
 * string properties
 */

static string getStringProperty(int prop)
{
    error("Unknown string property " + prop);
}

string getStringProperties(int *props)
{
    int sz, i;
    string *format;
    mixed *values;

    sz = sizeof(props);
    format = allocate(sz);
    values = allocate(sz * 2);
    for (i = 0; i < sz; i++) {
	format[i] = "it";
	values[i * 2] = props[i];
	values[i * 2 + 1] = getStringProperty(props[i]);
    }
    return serialize(implode(format, ""), values...);
}

/*
 * data properties
 */

static int getDataProperty(int prop)
{
    error("Unknown data property " + prop);
}

string getDataProperties(int *props)
{
    int sz, i, *values;
    string *format;

    sz = sizeof(props);
    format = allocate(sz);
    values = allocate_int(sz * 2);
    for (i = 0; i < sz; i++) {
	format[i] = "ii";
	values[i * 2] = props[i];
	values[i * 2 + 1] = getDataProperty(props[i]);
    }
    return serialize(implode(format, ""), values...);
}

/*
 * instance properties
 */

static int getInstanceProperty(int prop)
{
    error("Unknown instance property " + prop);
}

string getInstanceProperties(int *props)
{
    int sz, i, *values;
    string *format;

    sz = sizeof(props);
    format = allocate(sz);
    values = allocate_int(sz * 2);
    for (i = 0; i < sz; i++) {
	format[i] = "ii";
	values[i * 2] = props[i];
	values[i * 2 + 1] = getInstanceProperty(props[i]);
    }
    return serialize(implode(format, ""), values...);
}

/*
 * position properties
 */

static Position getPositionProperty(int prop)
{
    error("Unknown position property " + prop);
}

string getPositionProperties(int *props)
{
    int sz, i;
    string *values;

    sz = sizeof(props);
    values = allocate(sz * 2);
    for (i = 0; i < sz; i++) {
	values[i * 2] = serialize("i", props[i]);
	values[i * 2 + 1] = getPositionProperty(props[i])->transport();
    }
    return implode(values, "");
}
