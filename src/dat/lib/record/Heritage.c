# include "dat.h"


private int index;
private string name;			/* heritage name */
private int iconImage;
private int setupId;
private int envSetupId;
private int attributeCredits;
private int skillCredits;
private int *primaryStartAreas;
private int *secondaryStartAreas;
private int **skills;
private DatTemplate *templates;		/* class templates */
private DatAppearance *appearances;

/*
 * initialize heritage from portal.dat
 */
static void create(DatReader reader)
{
    int n, i;

    ({
	index,
	name,
	iconImage,
	setupId,
	envSetupId,
	attributeCredits,
	skillCredits,
	n
    }) = reader->read("ipiiiiic");

    /*
     * primary and secondary start areas
     */
    primaryStartAreas = reader->read("i", n);
    n = reader->read("c")[0];
    secondaryStartAreas = (n != 0) ? reader->read("i", n) : ({ });

    /*
     * skills
     */
    n = reader->read("c")[0];
    skills = allocate(n);
    for (i = 0; i < n; i++) {
	skills[i] = reader->read("iii");
    }

    /*
     * class templates
     */
    n = reader->read("c")[0];
    templates = allocate(n);
    for (i = 0; i < n; i++) {
	templates[i] = new DatTemplate(reader);
    }

    reader->skip(1);	/* 0x01 */

    /*
     * sex-specific appearance
     */
    n = reader->read("c")[0];
    appearances = allocate(n);
    for (i = 0; i < n; i++) {
	appearances[i] = new DatAppearance(reader);
    }
}


int index()			{ return index; }
string name()			{ return name; }
int iconImage()			{ return iconImage; }
int setupId()			{ return setupId; }
int envSetupId()		{ return envSetupId; }
int attributeCredits()		{ return attributeCredits; }
int skillCredits()		{ return skillCredits; }
int *primaryStartAreas()	{ return primaryStartAreas; }
int *secondaryStartAreas()	{ return secondaryStartAreas; }
int **skills()			{ return skills; }
DatTemplate *templates()	{ return templates; }
DatAppearance *appearances()	{ return appearances; }
