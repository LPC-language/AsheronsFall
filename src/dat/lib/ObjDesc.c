# include "ObjDesc.h"
# include "dat.h"

inherit ObjDesc;


/*
 * initialize from portal.dat
 */
static void create(DatReader reader)
{
    int objdesc, numPalettes, numTextures, numModels, i, paletteBaseId,
	**palettes, **textures, **models;

    reader->align();

    ({
	objdesc,
	numPalettes,
	numTextures,
	numModels
    }) = reader->read(headerLayout());

    palettes = allocate(numPalettes);
    if (numPalettes != 0) {
	paletteBase = reader->read("I");
	for (i = 0; i < numPalettes; i++) {
	    palettes[i] = reader->read("ccI");
	}
    }

    textures = allocate(numTextures);
    for (i = 0; i < numTextures; i++) {
	textures[i] = reader->read("cII");
    }

    models = allocate(numModels);
    for (i = 0; i < numModels; i++) {
	models[i] = reader->read("cI");
    }

    reader->align();

    ::create(paletteBase, palettes, textures, models);
}
