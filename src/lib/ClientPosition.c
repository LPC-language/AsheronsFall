# include "Position.h"

inherit Position;


/*
 * create a position from a blob
 */
static void create(string blob, int offset)
{
    int cell;
    float X, Y, Z, qW, qX, qY, qZ;

    ({
	offset,
	cell,
	X,
	Y,
	Z,
	qW,
	qX,
	qY,
	qZ
    }) = deSerialize(blob, offset, "ifffffff");
    ::create(cell, X, Y, Z, qW, qX, qY, qZ);
}
