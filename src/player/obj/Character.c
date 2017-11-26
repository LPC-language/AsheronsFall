# include <status.h>
# include <Time.h>
# include "Creature.h"
# include "User.h"
# include "Properties.h"

inherit Humanoid;


int *attributes;
int *vitalAttributes;
int *skills;
Account account;	/* account this character is in */
int deleteTimer;	/* delete timer handle */

/*
 * initialize character
 */
static void create(Account account, string name)
{
    ::create(0, name, ({ 100, 100, 100, 100, 100, 100 }), ({ 100, 100, 100 }),
	     ({ }));
    ::account = account;
}

/*
 * pending delete?
 */
int deleteTimer()
{
    if (deleteTimer != 0) {
	mixed *callouts;
	int sz, i;

	callouts = status(this_object(), O_CALLOUTS);
	for (sz = sizeof(callouts), i = 0; i < sz; i++) {
	    if (callouts[i][CO_HANDLE] == deleteTimer) {
		return callouts[i][CO_DELAY] + 1;
	    }
	}
    }
    return 0;
}

/*
 * start a delete with a delay of one hour
 */
int timedDelete()
{
    if (previous_object() == account && deleteTimer == 0) {
	deleteTimer = call_out("delete", 3600);
	return TRUE;
    }
    return FALSE;
}

/*
 * cancel a timed delete
 */
int cancelDelete()
{
    if (previous_object() == account && deleteTimer != 0) {
	remove_call_out(deleteTimer);
	deleteTimer = 0;
	return TRUE;
    }
    return FALSE;
}

/*
 * delete this character irrevocably
 */
static void delete()
{
    destruct_object(this_object());
}


string getAttribute(int attr)
{
    int incr;

    incr = attributes[attr];
    return serialize("iiiu", attr, incr, ::attribute(attr),
		     USER_SERVER->attributeXp(incr));
}

string getVitalAttribute(int vital)
{
    int incr;

    incr = vitalAttributes[vital];
    return serialize("iiiui", vital, incr, ::vitalAttribute(vital),
		     USER_SERVER->vitalXp(incr), ::vital(vital));
}

string getSkill(int skill)
{
    int statIncr, stat, incr;

    statIncr = skills[skill];
    stat = SKILL_STAT(statIncr);
    incr = SKILL_INCR(statIncr);
    return serialize("issiuiiD", skill, incr, TRUE, SKILL_STAT(statIncr),
		     USER_SERVER->skillXp(stat, incr), ::skill(skill), 0,
		     new Time(0, 0.0));
}


static int getBoolProperty(int prop)
{
    switch (prop) {
    case PROP_BOOL_IS_ADMIN:
    case PROP_BOOL_IS_ARCH:
    case PROP_BOOL_IS_SENTINEL:
    case PROP_BOOL_IS_ADVOCATE:
    case PROP_BOOL_IS_PSR:
    case PROP_BOOL_ACCOUNT_15_DAYS:
	return FALSE;

    case PROP_BOOL_ACTD_RECEIVED_ITEMS:
	return TRUE;

    default:
	return ::getBoolProperty(prop);
    }
}

static int getIntProperty(int prop)
{
    switch (prop) {
    case PROP_INT_COIN_VALUE:
	return 10000;

    case PROP_INT_CONTAINERS_CAPACITY:
	return 20;

    case PROP_INT_CREATION_TIMESTAMP:
	return 1485707122;

    case PROP_INT_PLAYER_KILLER_STATUS:
	return 2;

    case PROP_INT_HERITAGE_GROUP:
	return 1;

    case PROP_INT_MELEE_MASTERY:
	return 6;

    case PROP_INT_RANGED_MASTERY:
	return 8;

    case PROP_INT_ALLEGIANCE_RANK:
    case PROP_INT_HEALING_RESIST_RATING:
    case PROP_INT_DAMAGE_OVER_TIME:
    case PROP_INT_LUM_AUG_DAMAGE_RATING:
    case PROP_INT_LUM_AUG_DAMAGE_REDUCTION_RATING:
    case PROP_INT_LUM_AUG_CRIT_DAMAGE_RATING:
    case PROP_INT_LUM_AUG_CRIT_REDUCTION_RATING:
    case PROP_INT_LUM_AUG_SURGE_CHANCE_RATING:
    case PROP_INT_LUM_AUG_ITEM_MANA_USAGE:
    case PROP_INT_LUM_AUG_ITEM_MANA_GAIN:
    case PROP_INT_LUM_AUG_HEALING_RATING:
    case PROP_INT_LUM_AUG_SKILLED_CRAFT:
    case PROP_INT_LUM_AUG_SKILLED_SPEC:
    case PROP_INT_LUM_AUG_ALL_SKILLS:
    case PROP_INT_PK_DAMAGE_RATING:
    case PROP_INT_PK_DAMAGE_RESIST_RATING:
    case PROP_INT_ENLIGHTENMENT:
	return 0;

    default:
	return ::getIntProperty(prop);
    }
}

static float getLongProperty(int prop)
{
    switch (prop) {
    case PROP_LONG_AVAILABLE_LUMINANCE:
    case PROP_LONG_MAXIMUM_LUMINANCE:
	return 0.0;

    default:
	return ::getLongProperty(prop);
    }
}

static float getDoubleProperty(int prop)
{
    if (prop == PROP_DOUBLE_GLOBAL_XP_MOD) {
	return 1.0;
    }

    return ::getDoubleProperty(prop);
}


Account account()	{ return account; }
