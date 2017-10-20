# include "dat.h"


private string name;		/* name of this garment */
private int clothingTable;
private int class;		/* weenie class ID */

/*
 * initialize from portal.dat
 */
static void create(DatReader reader)
{
    ({
	name,
	clothingTable,
	class
    }) = reader->read("pii");
}


string name()		{ return name; }
int clothingTable()	{ return clothingTable; }
int class()		{ return class; }
