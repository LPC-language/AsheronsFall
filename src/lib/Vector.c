# include "Packet.h"
# include "Serialized.h"
# include "Vector.h"

inherit NetworkData;
inherit Serialized;


private float X, Y, Z;	/* coordinates or movement */

/*
 * layout of vector
 */
static string headerLayout()
{
    return "fff";
}

/*
 * size in transport
 */
int size()
{
    return 12;
}

/*
 * export as a blob
 */
string transport()
{
    return serialize(headerLayout(), X, Y, Z);
}

/*
 * create Vector
 */
static void create(float X, float Y, float Z)
{
    ::create(ND_DATA);

    ::X = X;
    ::Y = Y;
    ::Z = Z;
}


float X()	{ return X; }
float Y()	{ return Y; }
float Z()	{ return Z; }


/*
 * this == other
 */
int equals(Vector other)
{
    return (X == other->X() && Y == other->Y() && Z == other->Z());
}
