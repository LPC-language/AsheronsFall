# include "Message.h"
# include "Object.h"
# include "User.h"
# include "Creature.h"
# include "dat.h"


mapping characters;		/* characters to keep track of */
float *attributeXp;		/* attribute XP cost table */
float *vitalXp;			/* vital XP cost table */
float *trainedSkillXp;		/* trained skill XP cost table */
float *specializedSkillXp;	/* specialized skill XP cost table */
float *levelXp;			/* level XP cost table */
int *skillPoints;		/* skill points per level */

/*
 * initialize user/character server
 */
static void create()
{
    characters = ([ ]);

    compile_object(OBJECT_PATH(Character));
    compile_object(OBJECT_PATH(PrivateRemoveInstance));
    compile_object(OBJECT_PATH(PrivateUpdateInt));
    compile_object(OBJECT_PATH(UpdateInt));
    compile_object(OBJECT_PATH(PrivateUpdateLong));
    compile_object(OBJECT_PATH(PrivateUpdateBool));
    compile_object(OBJECT_PATH(UpdateBool));
    compile_object(OBJECT_PATH(PrivateUpdateDouble));
    compile_object(OBJECT_PATH(UpdateDouble));
    compile_object(OBJECT_PATH(UpdateString));
    compile_object(OBJECT_PATH(PrivateUpdateData));
    compile_object(OBJECT_PATH(UpdateData));
    compile_object(OBJECT_PATH(PrivateUpdateInstance));
    compile_object(OBJECT_PATH(UpdateInstance));
    compile_object(OBJECT_PATH(PrivateUpdatePosition));
    compile_object(OBJECT_PATH(PrivateUpdateSkill));
    compile_object(OBJECT_PATH(PrivateUpdateAttribute));
    compile_object(OBJECT_PATH(PrivateUpdateVital));
    compile_object(OBJECT_PATH(PrivateUpdateVitalLevel));
    compile_object(OBJECT_PATH(GenericMessage));
    compile_object(OBJECT_PATH(CharacterCreateResponse));
    compile_object(OBJECT_PATH(ClientCharacterDelete));
    compile_object(OBJECT_PATH(ClientCharacterCreate));
    compile_object(OBJECT_PATH(CharacterList));
    compile_object(OBJECT_PATH(CharacterError));
    compile_object(OBJECT_PATH(GameEvent));
    compile_object(OBJECT_PATH(ClientCharacterRestore));
    compile_object(OBJECT_PATH(ServerName));

    call_out("loadXpTables", 0);
}

static void loadXpTables()
{
    object datXpTable;

    datXpTable = clone_object(OBJECT_PATH(XpTable));
    attributeXp = datXpTable->attributes();
    vitalXp = datXpTable->vitals();
    trainedSkillXp = datXpTable->trainedSkills();
    specializedSkillXp = datXpTable->specializedSkills();
    levelXp = datXpTable->levels();
    skillPoints = datXpTable->skillPoints();
    destruct_object(datXpTable);
}

/*
 * is a name banned?
 */
int characterBanned(string name)
{
    return FALSE;
}

/*
 * find a character by name
 */
Character character(string name)
{
    mapping map;

    map = characters[name[.. 1]];
    return (map) ? map[name] : nil;
}

/*
 * add a new character
 */
void characterAdd(Character character)
{
    if (previous_program() == OBJECT_PATH(Account)) {
	string name;
	mapping map;

	name = character->name();
	map = characters[name[.. 1]];
	if (!map) {
	    map = characters[name[.. 1]] = ([ ]);
	}
	map[name] = character;
    }
}

/*
 * remove a character
 */
void characterRemove(Character character)
{
    if (previous_program() == OBJECT_PATH(Account)) {
	string name;
	mapping map;

	name = character->name();
	map = characters[name[.. 1]];
	map[name] = nil;
	if (map_sizeof(map) == 0) {
	    characters[name[.. 1]] = nil;
	}
    }
}


float attributeXp(int level)		{ return attributeXp[level]; }
float vitalXp(int level)		{ return vitalXp[level]; }
float levelXp(int level)		{ return levelXp[level]; }
int skillPoint(int level)		{ return skillPoints[level]; }

float skillXp(int stat, int level)
{
    switch (stat) {
    case SKILLSTAT_TRAINED:
	return trainedSkillXp[level];

    case SKILLSTAT_SPECIALIZED:
	return specializedSkillXp[level];

    default:
	return 0.0;
    }
}
