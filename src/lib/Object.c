# include <type.h>
# include <Time.h>
# include "Position.h"
# include "Serialized.h"
# include "properties.h"

/*
 * a client-visible object, not necessarily a physical object
 */

inherit Serialized;


private int class;		/* "weenie" class id */
private int id;			/* unique ID that identifies this object */
private mapping sequences;	/* sequences for this object */

/*
 * create object
 */
static void create(int class, int id)
{
    ::class = class;
    ::id = id;
    sequences = ([ ]);
}


static void setId(int id)	{ ::id = id; }

int class()			{ return class; }
int id()			{ return id; }


/* ========================================================================= *
 *				sequences				     *
 * ========================================================================= */

# define SEQ_INDEX(n)		((n) & 0xffff)
# define SEQ_MASK(n)		((n) >> 16)
# define SEQ_VALUE		0
# define SEQ_SEQUENCE		1

# define SEQ_INSTANCE		0xffff0001
# define SEQ_STATE		0xffff0002
# define SEQ_OBJDESC		0xffff0003
# define SEQ_POSITION		0xffff0004
# define SEQ_VECTOR		0xffff0005
# define SEQ_MOVEMENT		0xffff0006
# define SEQ_TELEPORT		0xffff0007
# define SEQ_FORCED		0xffff0008
# define SEQ_CONTROLLED		0xffff0009
# define SEQ_MOTION		0x7fff000a
# define SEQ_STACK		0xffff000b

# define BOOL(prop)		(0xff1000 + (prop))
# define INT(prop)		(0xff2000 + (prop))
# define LONG(prop)		(0xff3000 + (prop))
# define DOUBLE(prop)		(0xff4000 + (prop))
# define STRING(prop)		(0xff5000 + (prop))
# define DATA(prop)		(0xff6000 + (prop))
# define INSTANCE(prop)		(0xff7000 + (prop))
# define POSITION(prop)		(0xff8000 + (prop))
# define ATTR(prop)		(0xff9000 + (prop))
# define VITAL_ATTR(prop)	(0xffa000 + (prop))
# define VITAL(prop)		(0xffb000 + (prop))
# define SKILL(prop)		(0xffc000 + (prop))

/*
 * update sequence number if value has changed
 */
private int seq(int n, mixed value)
{
    mixed *arr;

    arr = sequences[SEQ_INDEX(n)];
    if (!arr) {
	sequences[SEQ_INDEX(n)] = arr = ({ value, 0 });
	return 0;
    } else if ((typeof(value) == T_OBJECT) ?
		value->equals(arr[SEQ_VALUE]) : (value == arr[SEQ_VALUE])) {
	return arr[SEQ_SEQUENCE] & SEQ_MASK(n);
    } else {
	arr[SEQ_VALUE] = value;
	return ++arr[SEQ_SEQUENCE] & SEQ_MASK(n);
    }
}

static int instanceSeq(int value)	{ return seq(SEQ_INSTANCE, value); }
static int stateSeq(int value)		{ return seq(SEQ_STATE, value); }
static int objDescSeq(int value)	{ return seq(SEQ_OBJDESC, value); }
static int positionSeq(int value)	{ return seq(SEQ_POSITION, value); }
static int vectorSeq(int value)		{ return seq(SEQ_VECTOR, value); }
static int movementSeq(int value)	{ return seq(SEQ_MOVEMENT, value); }
static int teleportSeq(int value)	{ return seq(SEQ_TELEPORT, value); }
static int forcedSeq(int value)		{ return seq(SEQ_FORCED, value); }
static int controlledSeq(int value)	{ return seq(SEQ_CONTROLLED, value); }

static int attributeSeq(int attr, int value)
{
    return seq(ATTR(attr), value);
}

static int vitalAttributeSeq(int vitalAttr, int value)
{
    return seq(VITAL_ATTR(vitalAttr), value);
}

static int vitalSeq(int vital, int value)
{
    return seq(VITAL(vital), value);
}

static int skillSeq(int skill, int value)
{
    return seq(SKILL(skill), value);
}

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
    int value;

    value = boolProperty(prop);
    return serialize("cii", seq(BOOL(prop), value), prop, value);
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
    int value;

    value = intProperty(prop);
    return serialize("cii", seq(INT(prop), value), prop, value);
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
    float value;

    value = longProperty(prop);
    return serialize("cil", seq(LONG(prop), value), prop, value);
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
    mixed value;

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
	value = timeProperty(prop);
	return serialize("ciD", seq(DOUBLE(prop), value), prop, value);
	break;

    default:
	value = doubleProperty(prop);
	return serialize("cid", seq(DOUBLE(prop), value), prop, value);
	break;
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
    string value;

    value = stringProperty(prop);
    return serialize("c", seq(STRING(prop), value)) +
	   serialize("it", prop, value);
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
    int value;

    value = dataProperty(prop);
    return serialize("cii", seq(DATA(prop), value), prop, value);
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
    int value;

    value = instanceProperty(prop);
    return serialize("cii", seq(INSTANCE(prop), value), prop, value);
}

string removeInstanceProperty(int prop)
{
    /* XXX does this reset the sequence? */
    return serialize("ci", seq(INSTANCE(prop), nil), prop);
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
    Position value;

    value = positionProperty(prop);
    return serialize("ci", seq(POSITION(prop), value), prop) +
	   value->transport();
}
