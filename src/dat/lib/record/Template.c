# include "dat.h"


private string name;
private int iconImage;
private int title;
private int *attributes;
private int *trainedSkills;
private int *specializedSkills;

/*
 * initialize from portal.dat
 */
static void create(DatReader reader)
{
    int n;

    ({
	name,
	iconImage,
	title
    }) = reader->read("pii");

    attributes = reader->read("iiiiii");

    n = reader->read("c")[0];
    trainedSkills = (n == 0) ? ({ }) : reader->read("i", n);

    n = reader->read("c")[0];
    specializedSkills = (n == 0) ? ({ }) : reader->read("i", n);
}


string name()			{ return name; }
int iconImage()			{ return iconImage; }
int title()			{ return title; }
int *attributes()		{ return attributes; }
int *trainedSkills()		{ return trainedSkills; }
int *specializedSkills()	{ return specializedSkills; }
