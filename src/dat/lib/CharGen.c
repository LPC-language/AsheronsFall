# include "dat.h"


private int id;				/* 0x0e000002 */
private mixed *starterAreas;		/* list of starter areas */
private DatHeritage *heritageGroups;	/* list of heritage groups */

/*
 * read from portal.dat
 */
static void create(DatReader reader)
{
    int n, i;
    string name;

    ({
	id,
	id,
	n
    }) = reader->read("iic");

    /*
     * 4 starter towns with 5 training dungeons each, one olthoi spawn
     */
    starterAreas = allocate(n);
    for (i = 0; i < n; i++) {
	int nLocations, j;
	DatPosition *locations;

	({ name, nLocations }) = reader->read("pc");
	locations = allocate(nLocations);
	for (j = 0; j < nLocations; j++) {
	    locations[j] = new DatPosition(reader);
	}
	starterAreas[i] = ({ name, locations });
    }

    reader->skip(1);	/* 0x01 */

    /*
     * heritage groups
     */
    n = reader->read("c")[0];
    heritageGroups = allocate(n);
    for (i = 0; i < n; i++) {
	heritageGroups[i] = new DatHeritage(reader);
    }
}


int id()			{ return id; }
mixed *starterAreas()		{ return starterAreas; }
DatHeritage *heritageGroups()	{ return heritageGroups; }
