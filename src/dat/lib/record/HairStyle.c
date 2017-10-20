# include "dat.h"


private int iconImage;
private int bald;
private int alternateSetup;
DatObjDesc objDesc;

/*
 * initialize hairstyle from portal.dat
 */
static void create(DatReader reader)
{
    ({
	iconImage,
	bald,
	alternateSetup
    }) = reader->read("ici");
    objDesc = new DatObjDesc(reader);
}


int iconImage()		{ return iconImage; }
int bald()		{ return bald; }
int alternateSetup()	{ return alternateSetup; }
DatObjDesc objDesc()	{ return objDesc; }
