# include <status.h>
# include <Time.h>
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
static void setBoolProperty(int prop, int value)
{
    error("Unknown bool property " + prop);
}

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

int setBoolProperties(string blob, int offset, int n)
{
    int *list, i;

    list = deSerialize(blob, offset, "ii", n);
    for (i = 0; i < n; i++) {
	setBoolProperty(list[i * 2 + 1], list[i * 2 + 2]);
    }
    return list[0];
}

/*
 * int properties
 */

static void setIntProperty(int prop, int value)
{
    error("Unknown int property " + prop);
}

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

int setIntProperties(string blob, int offset, int n)
{
    int *list, i;

    list = deSerialize(blob, offset, "ii", n);
    for (i = 0; i < n; i++) {
	setIntProperty(list[i * 2 + 1], list[i * 2 + 2]);
    }
    return list[0];
}

/*
 * long properties
 */

static void setLongProperty(int prop, float value)
{
    error("Unknown long property " + prop);
}

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

int setLongProperties(string blob, int offset, int n)
{
    mixed *list;
    int i;

    list = deSerialize(blob, offset, "il", n);
    for (i = 0; i < n; i++) {
	setLongProperty(list[i * 2 + 1], list[i * 2 + 2]);
    }
    return list[0];
}

/*
 * double properties
 */

static void setDoubleProperty(int prop, float value)
{
    error("Unknown double property " + prop);
}

static float getDoubleProperty(int prop)
{
    error("Unknown double property " + prop);
}

static void setTimeProperty(int prop, Time time)
{
    error("Unknown time property " + prop);
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

int setDoubleProperties(string blob, int offset, int n)
{
    int i, prop;

    for (i = 0; i < n; i++) {
	({ offset, prop }) = deSerialize(blob, offset, "i");
	if (isTimeProperty(prop)) {
	    Time time;

	    ({ offset, time }) = deSerialize(blob, offset, "D");
	    setTimeProperty(prop, time);
	} else {
	    float value;

	    ({ offset, value }) = deSerialize(blob, offset, "d");
	    setDoubleProperty(prop, value);
	}
    }
    return offset;
}

/*
 * string properties
 */

static void setStringProperty(int prop, string value)
{
    error("Unknown string property " + prop);
}

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

int setStringProperties(string blob, int offset, int n)
{
    mixed *list;
    int i;

    list = deSerialize(blob, offset, "it", n);
    for (i = 0; i < n; i++) {
	setStringProperty(list[i * 2 + 1], list[i * 2 + 2]);
    }
    return list[0];
}

/*
 * data properties
 */

static void setDataProperty(int prop, int value)
{
    error("Unknown data property " + prop);
}

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

int setDataProperties(string blob, int offset, int n)
{
    int *list, i;

    list = deSerialize(blob, offset, "ii", n);
    for (i = 0; i < n; i++) {
	setDataProperty(list[i * 2 + 1], list[i * 2 + 2]);
    }
    return list[0];
}

/*
 * instance properties
 */

static void setInstanceProperty(int prop, int value)
{
    error("Unknown instance property " + prop);
}

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

int setInstanceProperties(string blob, int offset, int n)
{
    int *list, i;

    list = deSerialize(blob, offset, "ii", n);
    for (i = 0; i < n; i++) {
	setInstanceProperty(list[i * 2 + 1], list[i * 2 + 2]);
    }
    return list[0];
}
