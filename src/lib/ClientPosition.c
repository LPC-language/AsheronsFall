# include "Position.h"

inherit Position;


/*
 * create a position from a blob
 */
static void create(string blob, int offset)
{
    int cell;
    float X, Y, Z, rotW, rotX, rotY, rotZ;

    ({
	offset,
	cell,
	X,
	Y,
	Z,
	rotW,
	rotX,
	rotY,
	rotZ
    }) = deSerialize(blob, offset, headerLayout());
    ::create(cell, X, Y, Z, rotW, rotX, rotY, rotZ);
}
