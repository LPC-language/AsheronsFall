# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


private int cell;			/* environment */
private float X, Y, Z;			/* location */
private float rotW, rotX, rotY, rotZ;	/* orientation */

/*
 * layout of position
 */
static string headerLayout()
{
    return "ifffffff";
}

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
    return serialize(headerLayout(), cell, X, Y, Z, rotW, rotX, rotY, rotZ);
}

/*
 * initialize
 */
static void create(varargs int cell, float X, float Y, float Z,
		   float rotW, float rotX, float rotY, float rotZ)
{
    ::create(ND_DATA);
    ::cell = cell;
    ::X = X;
    ::Y = Y;
    ::Z = Z;
    ::rotW = rotW;
    ::rotX = rotX;
    ::rotY = rotY;
    ::rotZ = rotZ;
}


int cell()	{ return cell; }
float X()	{ return X; }
float Y()	{ return Y; }
float Z()	{ return Z; }
float rotW()	{ return rotW; }
float rotX()	{ return rotX; }
float rotY()	{ return rotY; }
float rotZ()	{ return rotZ; }
