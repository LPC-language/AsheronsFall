# include "Serialized.h"
# include "Packet.h"

inherit NetworkData;
inherit Serialized;


# define OBJDESC	0x11	/* first byte in transport */

private int paletteBaseId;
private int **palettes;
private int **textures;
private int **models;
private int size;		/* size, computed by transport() */

/*
 * layout of header
 */
static string headerLayout()
{
    return "cccc";
}

/*
 * size in transport
 */
int size()
{
    return size;
}

/*
 * transport as a blob
 */
string transport()
{
    string *blobs, blob;
    int n, sz, i;

    blobs = allocate(1 + sizeof(palettes) + sizeof(textures) + sizeof(models));
    blobs[0] = serialize(headerLayout(), OBJDESC, sizeof(palettes),
			 sizeof(textures), sizeof(models));

    n = 0;
    sz = sizeof(palettes);
    if (sz != 0) {
	blobs[0] += serialize("I", paletteBaseId);
	for (i = 0; i < sz; i++) {
	    blobs[++n] += serialize("Icc", palettes[i]...);
	}
    }
    for (sz = sizeof(textures), i = 0; i < sz; i++) {
	blobs[++n] = serialize("cII", textures[i]...);
    }
    for (sz = sizeof(models), i = 0; i < sz; i++) {
	blobs[++n] = serialize("cI", models[i]...);
    }

    blob = serializeAlign(implode(blobs, ""));
    size = strlen(blob);
    return blob;
}

/*
 * initialize
 */
static void create(int paletteBaseId, int **palettes, int **textures,
		   int **models)
{
    ::create(ND_DATA);
    ::paletteBaseId = paletteBaseId;
    ::palettes = palettes;
    ::textures = textures;
    ::models = models;
}


int paletteBaseId()	{ return paletteBaseId; }
int **palettes()	{ return palettes; }
int **textures()	{ return textures; }
int **models()		{ return models; }
