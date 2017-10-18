/*
 * BTree used by DatImage internally.  This object knows how to construct
 * itself, but otherwise is a simple data container for use by DatImage.
 */

# include "Serialized.h"
# include "dat.h"

# define ENTRY_SIZE	24	/* size of BTree entry, 6 * sizeof(uint32_t) */


inherit Serialized;

private int *branches;
private DatItem *entries;

static void create(string node)
{
    int offset, n, i;

    branches = deSerialize(node, 0, "i", 62)[1 ..];
    ({ offset, n }) = deSerialize(node, 248, "i");
    if (branches[0] == 0) {
	branches = ({ });	/* leaf node */
    } else {
	branches = branches[.. n];
    }
    entries = allocate(n);
    for (i = 0; i < n; i++) {
	entries[i] = new DatItem(node, offset);
	offset += ENTRY_SIZE;
    }
}

int *branches()		{ return branches; }
DatItem *entries()	{ return entries; }
