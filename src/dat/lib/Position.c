# include "Position.h"
# include "dat.h"

inherit Position;


/*
 * initialize from portal.dat
 */
static void create(DatReader reader)
{
    int cell;
    float X, Y, Z, rotW, rotX, rotY, rotZ;

    ({
	cell,
	X,
	Y,
	Z,
	rotW,
	rotX,
	rotY,
	rotZ
    }) = reader->read(headerLayout());
    ::create(cell, X, Y, Z, rotW, rotX, rotY, rotZ);
}
