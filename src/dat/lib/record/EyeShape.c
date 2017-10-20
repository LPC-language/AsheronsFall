# include "dat.h"


private int iconImage;
private int iconImageBald;
private DatObjDesc objDesc;
private DatObjDesc objDescBald;

/*
 * initialize eyeshape from portal.dat
 */
static void create(DatReader reader)
{
    ({
	iconImage,
	iconImageBald,
    }) = reader->read("ii");
    objDesc = new DatObjDesc(reader);
    objDescBald = new DatObjDesc(reader);
}


int iconImage()			{ return iconImage; }
int iconImageBald()		{ return iconImageBald; }
DatObjDesc objDesc()		{ return objDesc; }
DatObjDesc objDescBald()	{ return objDescBald; }
