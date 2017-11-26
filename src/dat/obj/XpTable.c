# include "dat.h"


private int id;				/* 0x0e000018 */
private float *attributes;		/* attributes experience */
private float *vitals;			/* vitals experience */
private float *trainedSkills;		/* trained skills experience */
private float *specializedSkills;	/* specialized skills experience */	
private float *levels;			/* levels experience */
private int *skillPoints;		/* skill points per level */

/*
 * read from portal.dat
 */
static void create()
{
    object portal;
    DatReader reader;
    int attributeLevels, vitalLevels, trainedSkillLevels,
	specializedSkillLevels, numLevels;

    portal = DAT_SERVER->portal();
    reader = portal->getItemReader(portal->iterator(DAT_XPTABLE)->next());
    ({
	id,
	attributeLevels,
	vitalLevels,
	trainedSkillLevels,
	specializedSkillLevels,
	numLevels
    }) = reader->read("iiiiii");

    attributes = reader->read("u", attributeLevels + 1);
    vitals = reader->read("u", vitalLevels + 1);
    trainedSkills = reader->read("u", trainedSkillLevels + 1);
    specializedSkills = reader->read("u", specializedSkillLevels + 1);
    levels = reader->read("l", numLevels + 1);
    skillPoints = reader->read("i", numLevels + 1);
}


int id()			{ return id; }
float *attributes()		{ return attributes; }
float *vitals()			{ return vitals; }
float *trainedSkills()		{ return trainedSkills; }
float *specializedSkills()	{ return specializedSkills; }
float *levels()			{ return levels; }
int *skillPoints()		{ return skillPoints; }
