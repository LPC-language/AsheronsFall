# include "Serialized.h"

# define ENTRY_SIZE	24

inherit Serialized;

private int *branches;
private int **entries;

static void create(string node)
{
    int n, i, offset;

    branches = deSerialize(node[.. 247], "i", 62);
    ({ n }) = deSerialize(node[248 .. 251], "i");
    if (branches[0] == 0) {
	branches = ({ });
    } else {
	branches = branches[.. n];
    }
    entries = allocate(n);
    for (offset = 252, i = 0; i < n; offset += ENTRY_SIZE, i++) {
	entries[i] = deSerialize(node[offset ..  offset + ENTRY_SIZE - 1],
				 "iiiiii", ENTRY_SIZE);
    }
}

int *branches()	{ return branches; }
int **entries()	{ return entries; }
