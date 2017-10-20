# include "dat.h"


private int iconImage;
private DatObjDesc objDesc;

/*
 * initialize facial feature from portal.dat
 */
static void create(DatReader reader)
{
    iconImage = reader->read("i")[0];
    objDesc = new DatObjDesc(reader);
}


int iconImage()			{ return iconImage; }
DatObjDesc objDesc()		{ return objDesc; }
