/*
 * BTree used by DatImage internally.  This object knows how to construct
 * itself, but otherwise is a simple data container for use by DatImage.
 */

# include "Serialized.h"
# include "Dat.h"

# define ENTRY_SIZE	24	/* size of BTree entry, 6 * sizeof(uint32_t) */


inherit Serialized;

private int *branches;
private DatItem *entries;

static void create(string node)
{
    int n, i, offset;

    branches = deSerialize(node[.. 247], "i", 62);
    ({ n }) = deSerialize(node[248 .. 251], "i");
    if (branches[0] == 0) {
	branches = ({ });	/* leaf node */
    } else {
	branches = branches[.. n];
    }
    entries = allocate(n);
    for (offset = 252, i = 0; i < n; offset += ENTRY_SIZE, i++) {
	entries[i] = new DatItem(node[offset .. offset + ENTRY_SIZE - 1]);
    }
}

int *branches()		{ return branches; }
DatItem *entries()	{ return entries; }
