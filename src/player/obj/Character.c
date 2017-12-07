# include <status.h>
# include <Time.h>
# include "Creature.h"
# include "User.h"
# include "properties.h"

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
    ::create(0, name, ({ 40, 30, 100, 100, 50, 10 }), ({ 0, 0, 0 }),
	     ({
		0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10, 0,
		0, 0, 0, 0, 0, 0
	     }));
    ::account = account;
    attributes = allocate_int(6);
    vitalAttributes = allocate_int(3);
    skills =  ({
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_SPECIALIZED, 0),	/* melee d */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* missile d */
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_SPECIALIZED, 0),	/* arcane */
		SKILL_STATINCR(SKILLSTAT_TRAINED, 5),		/* magic d */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* mana conv */
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* item t */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* assess p */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* deception */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* healing */
		SKILL_STATINCR(SKILLSTAT_TRAINED, 5),		/* jump */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* lockpick */
		SKILL_STATINCR(SKILLSTAT_TRAINED, 5),		/* run */
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* assess c */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* weapon t */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* armour t */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* magic t */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* creature */
		SKILL_STATINCR(SKILLSTAT_TRAINED, 5),		/* item */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* life */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* war */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* leadership */
		SKILL_STATINCR(SKILLSTAT_TRAINED, 5),		/* loyalty */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* fletching */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* alchemy */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* cooking */
		SKILL_STATINCR(SKILLSTAT_TRAINED, 5),		/* salvaging */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* 2handed */
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* void */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* heavy w */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* light w */
		SKILL_STATINCR(SKILLSTAT_SPECIALIZED, 0),	/* finesse w */
		SKILL_STATINCR(SKILLSTAT_SPECIALIZED, 0),	/* missile w */
		SKILL_STATINCR(SKILLSTAT_TRAINED, 5),		/* shield */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* dual */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* reckless */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* sneak a */
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0),		/* dirty f */
		SKILL_STATINCR(SKILLSTAT_INACTIVE, 0),
		SKILL_STATINCR(SKILLSTAT_UNTRAINED, 0)		/* summoning */
    });
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

    incr = attributes[attr - 1];
    return serialize("iiu", incr, ::attribute(attr),
		     USER_SERVER->attributeXp(incr));
}

string getVitalAttribute(int vital)
{
    int incr;

    incr = vitalAttributes[vital - 1];
    return serialize("iiui", incr, ::vitalAttribute(vital),
		     USER_SERVER->vitalXp(incr), ::vital(vital));
}

string getSkill(int skill)
{
    int statIncr, stat, incr;

    statIncr = skills[skill - 1];
    stat = SKILL_STAT(statIncr);
    incr = SKILL_INCR(statIncr);
    return serialize("issiuiiD", skill, incr, TRUE, stat,
		     USER_SERVER->skillXp(stat, incr), ::skill(skill), 0,
		     new Time(0, 0.0));
}

int *displayTitles()
{
    return ({ 9 });
}

int *titleList()
{
    return ({ 9 });
}

# define OPTIONLIST_SHORTCUT			0x0001
# define OPTIONLIST_SQUELCH_LIST		0x0002
# define OPTIONLIST_5_SPELL_TABS		0x0004
# define OPTIONLIST_DESIRED_COMPS		0x0008
# define OPTIONLIST_7_SPELL_TABS		0x0010
# define OPTIONLIST_SPELLBOOK_FILTERS		0x0020
# define OPTIONLIST_OPTIONS2			0x0040
# define OPTIONLIST_TIMESTAMP_FORMAT		0x0080
# define OPTIONLIST_PROPERTIES			0x0100
# define OPTIONLIST_INTERFACE			0x0200
# define OPTIONLIST_8_SPELL_TABS		0x0400

# define OPTION1_AUTO_REPEAT_ATTACKS		0x00000002
# define OPTION1_IGNORE_ALLEGIANCE_REQUESTS	0x00000004
# define OPTION1_IGNORE_FELLOWSHIP_REQUESTS	0x00000008
# define OPTION1_ACCEPT_ITEMS			0x00000040
# define OPTION1_KEEP_COMBAT_TARGETS_IN_VIEW	0x00000080
# define OPTION1_3D_TOOLTIPS			0x00000100
# define OPTION1_ATTEMPT_TO_DECEIVE		0x00000200
# define OPTION1_RUN_AS_DEFAULT_MOVEMENT	0x00000400
# define OPTION1_STAY_IN_CHAT_MODE		0x00000800
# define OPTION1_ADVANCED_COMBAT_UI		0x00001000
# define OPTION1_AUTO_TARGET			0x00002000
# define OPTION1_VIVID_TARGET_INDICATOR		0x00008000
# define OPTION1_DISABLE_WEATHER		0x00010000
# define OPTION1_IGNORE_TRADE_REQUESTS		0x00020000
# define OPTION1_SHARE_FELLOWSHIP_EXPERIENCE	0x00040000
# define OPTION1_ACCEPT_CORPSE_LOOT_PERMISSIONS	0x00080000
# define OPTION1_SHARE_FELLOWSHIP_LOOT		0x00100000
# define OPTION1_SIDE_BY_SIDE_VITALS		0x00200000
# define OPTION1_COORDINATES			0x00400000
# define OPTION1_SPELL_DURATIONS		0x00800000
# define OPTION1_DISABLE_HOUSE_EFFECTS		0x02000000
# define OPTION1_DRAGGING_ITEM_OPENS_TRADE	0x04000000
# define OPTION1_ALLEGIANCE_LOGONS		0x08000000
# define OPTION1_CHARGE_ATTACK			0x10000000
# define OPTION1_AUTO_ACCEPT_FELLOWSHIP		0x20000000
# define OPTION1_ALLEGIANCE_CHAT		0x40000000
# define OPTION1_CRAFTING_CHANCE_DIALOG		0x80000000

# define OPTION2_ALWAYS_DAYLIGHT_OUTDOORS	0x00000001
# define OPTION2_SHOW_DATE_OF_BIRTH		0x00000002
# define OPTION2_SHOW_CHESS_RANK		0x00000004
# define OPTION2_SHOW_FISHING_SKILL		0x00000008
# define OPTION2_SHOW_DEATHS			0x00000010
# define OPTION2_SHOW_AGE			0x00000020
# define OPTION2_TIMESTAMPS			0x00000040
# define OPTION2_SALVAGE_MULTIPLE_MATERIALS	0x00000080
# define OPTION2_GENERAL_CHAT			0x00000100
# define OPTION2_TRADE_CHAT			0x00000200
# define OPTION2_LFG_CHAT			0x00000400
# define OPTION2_RP_CHAT			0x00000800
# define OPTION2_APPEAR_OFFLINE			0x00001000
# define OPTION2_SHOW_NUMBER_OF_TITLES		0x00002000
# define OPTION2_ITEMS_TO_MAIN_PACK		0x00004000
# define OPTION2_LEAD_MISSILE_TARGETS		0x00008000
# define OPTION2_FAST_MISSILES			0x00010000
# define OPTION2_FILTER_LANGUAGE		0x00020000
# define OPTION2_CONFIRM_RARE_GEM_USE		0x00040000
# define OPTION2_SOCIETY_CHAT			0x00080000
# define OPTION2_SHOW_HEAD_GEAR			0x00100000
# define OPTION2_DISABLE_DISTANCE_FOG		0x00200000
# define OPTION2_MOUSE_TURNING			0x00400000
# define OPTION2_SHOW_CLOAK			0x00800000
# define OPTION2_LOCK_UI			0x01000000
# define OPTION2_PK_DEATH_MESSAGES		0x02000000

string options()
{
    int flags, options1, options2;

    flags = OPTIONLIST_SPELLBOOK_FILTERS | OPTIONLIST_OPTIONS2 |
	    OPTIONLIST_8_SPELL_TABS;
    options1 = OPTION1_AUTO_REPEAT_ATTACKS |
	       OPTION1_IGNORE_FELLOWSHIP_REQUESTS |
	       OPTION1_ACCEPT_ITEMS |
	       OPTION1_3D_TOOLTIPS |
	       OPTION1_RUN_AS_DEFAULT_MOVEMENT |
	       OPTION1_AUTO_TARGET |
	       OPTION1_VIVID_TARGET_INDICATOR |
	       OPTION1_SHARE_FELLOWSHIP_EXPERIENCE |
	       OPTION1_COORDINATES |
	       OPTION1_SPELL_DURATIONS |
	       OPTION1_CHARGE_ATTACK |
	       OPTION1_ALLEGIANCE_CHAT;
    options2 = OPTION2_GENERAL_CHAT |
	       OPTION2_TRADE_CHAT |
	       OPTION2_LFG_CHAT |
	       OPTION2_LEAD_MISSILE_TARGETS |
	       OPTION2_CONFIRM_RARE_GEM_USE |
	       OPTION2_SHOW_HEAD_GEAR |
	       OPTION2_SHOW_CLOAK |
	       OPTION2_PK_DEATH_MESSAGES;
    return serialize("iiiiiiiiiiii", flags, options1, 0, 0, 0, 0, 0, 0, 0, 0,
		     0x3fff, options2);
}


static int boolProperty(int prop)
{
    switch (prop) {
    case PROP_BOOL_IS_ADMIN:
    case PROP_BOOL_IS_ARCH:
    case PROP_BOOL_IS_SENTINEL:
    case PROP_BOOL_IS_ADVOCATE:
    case PROP_BOOL_IS_PSR:
	return FALSE;

    case PROP_BOOL_ACTD_RECEIVED_ITEMS:
    case PROP_BOOL_ACCOUNT_15_DAYS:
	return TRUE;

    default:
	return ::boolProperty(prop);
    }
}

static int intProperty(int prop)
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
	return ::intProperty(prop);
    }
}

static float longProperty(int prop)
{
    switch (prop) {
    case PROP_LONG_AVAILABLE_LUMINANCE:
    case PROP_LONG_MAXIMUM_LUMINANCE:
	return 0.0;

    default:
	return ::longProperty(prop);
    }
}

static float doubleProperty(int prop)
{
    if (prop == PROP_DOUBLE_GLOBAL_XP_MOD) {
	return 1.0;
    }

    return ::doubleProperty(prop);
}


Account account()	{ return account; }
