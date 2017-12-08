# include "Vector.h"

inherit Vector;


private int cell;		/* environment */
private float qW, qX, qY, qZ;	/* orientation */

/*
 * size in transport
 */
int size()
{
    return 32;
}

/*
 * export as a blob
 */
string transport()
{
    return serialize("i", cell) + ::transport() +
	   serialize("ffff", qW, qX, qY, qZ);
}

/*
 * initialize position
 */
static void create(varargs int cell, float X, float Y, float Z,
		   float qW, float qX, float qY, float qZ)
{
    ::create(X, Y, Z);
    ::cell = cell;
    ::qW = qW;
    ::qX = qX;
    ::qY = qY;
    ::qZ = qZ;
}


int cell()	{ return cell; }
float qW()	{ return qW; }
float qX()	{ return qX; }
float qY()	{ return qY; }
float qZ()	{ return qZ; }
