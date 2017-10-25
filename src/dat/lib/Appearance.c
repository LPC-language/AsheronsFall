# include "ObjDesc.h"
# include "dat.h"


private int id;
private string name;			/* male of female */
private int scale;			/* size as a percentage */
private int setupId;
private int soundTable;
private int iconImage;
private int basePalette;
private int skinPaletteSet;
private int physicsTable;
private int motionTable;
private int combatTable;
private DatObjDesc objDesc;
private int *hairColours;
private DatHairStyle *hairStyles;
private int *eyeColours;
private DatEyeShape *eyeShapes;
private DatFeature *noseShapes;
private DatFeature *mouthShapes;
private DatGarment *headgear;
private DatGarment *shirts;
private DatGarment *pants;
private DatGarment *footwear;
private int *clothingColours;

/*
 * initialize appearance
 */
static void create(DatReader reader)
{
    int n, i;

    ({
	id,
	name,
	scale,
	setupId,
	soundTable,
	iconImage,
	basePalette,
	skinPaletteSet,
	physicsTable,
	motionTable,
	combatTable
    }) = reader->read("ipiiiiiiiii");

    /* basic shape */
    objDesc = new DatObjDesc(reader);

    /* hair */
    n = reader->read("c")[0];
    hairColours = (n != 0) ? reader->read("i", n) : ({ });
    n = reader->read("c")[0];
    hairStyles = allocate(n);
    for (i = 0; i < n; i++) {
	hairStyles[i] = new DatHairStyle(reader);
    }

    /* eyes */
    n = reader->read("c")[0];
    eyeColours = (n != 0) ? reader->read("i", n) : ({ });
    n = reader->read("c")[0];
    eyeShapes = allocate(n);
    for (i = 0; i < n; i++) {
	eyeShapes[i] = new DatEyeShape(reader);
    }

    /* nose */
    n = reader->read("c")[0];
    noseShapes = allocate(n);
    for (i = 0; i < n; i++) {
	noseShapes[i] = new DatFeature(reader);
    }

    /* mouth */
    n = reader->read("c")[0];
    mouthShapes = allocate(n);
    for (i = 0; i < n; i++) {
	mouthShapes[i] = new DatFeature(reader);
    }

    /* clothing */
    n = reader->read("c")[0];
    headgear = allocate(n);
    for (i = 0; i < n; i++) {
	headgear[i] = new DatGarment(reader);
    }
    n = reader->read("c")[0];
    shirts = allocate(n);
    for (i = 0; i < n; i++) {
	shirts[i] = new DatGarment(reader);
    }
    n = reader->read("c")[0];
    pants = allocate(n);
    for (i = 0; i < n; i++) {
	pants[i] = new DatGarment(reader);
    }
    n = reader->read("c")[0];
    footwear = allocate(n);
    for (i = 0; i < n; i++) {
	footwear[i] = new DatGarment(reader);
    }
    n = reader->read("c")[0];
    clothingColours = (n != 0) ? reader->read("i", n) : ({ });
}


int id()			{ return id; }
string name()			{ return name; }
int scale()			{ return scale; }
int setupId()			{ return setupId; }
int soundTable()		{ return soundTable; }
int iconImage()			{ return iconImage; }
int basePalette()		{ return basePalette; }
int skinPaletteSet()		{ return skinPaletteSet; }
int physicsTable()		{ return physicsTable; }
int motionTable()		{ return motionTable; }
int combatTable()		{ return combatTable; }
DatObjDesc objDesc()		{ return objDesc; }
int *hairColours()		{ return hairColours; }
DatHairStyle *hairStyles()	{ return hairStyles; }
int *eyeColours()		{ return eyeColours; }
DatEyeShape *eyeShapes()	{ return eyeShapes; }
DatFeature *noseShapes()	{ return noseShapes; }
DatFeature *mouthShapes()	{ return mouthShapes; }
DatGarment *headgear()		{ return headgear; }
DatGarment *shirts()		{ return shirts; }
DatGarment *pants()		{ return pants; }
DatGarment *footwear()		{ return footwear; }
int *clothingColours()		{ return clothingColours; }
