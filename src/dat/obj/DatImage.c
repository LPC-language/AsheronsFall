/*
 * DAT image reader.  After creation, use an iterator to retrieve all the
 * items, sorted by ID.
 */

# include <String.h>
# include <Iterator.h>
# include "Serialized.h"
# include "BTree.h"
# include "Dat.h"

inherit Iterable;
inherit Serialized;


string fileName;			/* name of the DAT file */
BTree root;				/* btree of DAT items */

# define HEADER_OFFSET	320		/* offet of header in DAT image */
# define FILE_TYPE	0x5442		/* file type magic number */
# define FREE_BLOCK	0x80000000	/* upper bit in block signals unused */

int fileType;				/* 'TB' */
int blockSize;				/* size of a block */
int fileSize;				/* size of the DAT file */
int dataSet;
int dataSubset;
int freeHead;
int freeTail;
int freeCount;
int blockBTree;				/* block of BTree root node */

/*
 * Load the header from the DAT image, after which the BTrees and DatItems
 * can be retrieved.
 */
private void loadHeader()
{
    string header;
    int offset;

    header = read_file(fileName, HEADER_OFFSET, 36);
    if (!header) {
	error("Missing DAT file: " + fileName);
    }
    ({
	offset,
	fileType,
	blockSize,
	fileSize,
	dataSet,
	dataSubset,
	freeHead,
	freeTail,
	freeCount,
	blockBTree
    }) = deSerialize(header, 0, "iiiiiiiii");

    /*
     * Perform some minimal sanity checks.
     */
    if (fileType != FILE_TYPE) {
	error("Not a DAT file");
    }
    if (fileSize != file_info(fileName)[0]) {
	error("Invalid length in DAT header");
    }
}

/*
 * load a single block from the DAT image (with some validity checks)
 */
private string getBlock(int block)
{
    string chunk;

    if (block & FREE_BLOCK) {
	error("Free block");
    }
    if (block < 1024 || block > fileSize - blockSize || block % blockSize != 0)
    {
	error("Invalid block");
    }
    chunk = read_file(fileName, block, blockSize);
    if (strlen(chunk) != blockSize) {
	error("Short block");
    }

    return chunk;
}

/*
 * load data from chained blocks, and store it in a StringBuffer
 */
private StringBuffer getData(int block, int length)
{
    StringBuffer buffer;
    int n, offset;
    string chunk;

    buffer = new StringBuffer;
    if (length != 0) {
	for (n = (length - 1) / (blockSize - 4); n != 0; --n) {
	    chunk = getBlock(block);
	    ({ offset, block }) = deSerialize(chunk, 0, "i");
	    buffer->append(chunk[offset ..]);
	}

	chunk = getBlock(block);
	buffer->append(chunk[4 .. (length - 1) % (blockSize - 4) + 4]);
    }
    return buffer;
}

/*
 * load a BTree from the DAT image
 */
private BTree getBTree(int block)
{
    return new BTree(getData(block, BTREE_SIZE)->chunk());
}

/*
 * load the data for a DatItem
 */
StringBuffer getItemData(DatItem item)
{
    return getData(item->block(), item->length());
}

/*
 * get a reader for a DatItem
 */
DatReader getItemReader(DatItem item)
{
    return new DatReader(getItemData(item));
}


/*
 * initialize the image handler: load header and root BTree
 */
static void create(string file)
{
    fileName = file;
    loadHeader();
    root = getBTree(blockBTree);
}

string fileName() { return fileName; }

/*
 * reset iterator to initial state
 */
mixed iteratorStart(mixed from, mixed to)
{
    mixed *stack;
    float index, id;
    int intId, low, high, mid;
    BTree node;
    DatItem *entries;
    int *branches;

    if (!from) {
	return ({ to, root, 0 });
    }

    /*
     * find the starting entry in the BTree
     */
    stack = ({ to });
    intId = from;
    index = ldexp((float) (intId >> 1), 1) + (float) (intId & 1);
    for (node = root; ; node = getBTree(branches[mid])) {
	entries = node->entries();
	high = sizeof(entries);
	low = 0;
	while (low < high) {
	    mid = (low + high) >> 1;
	    intId = entries[mid]->id();
	    id = ldexp((float) (intId >> 1), 1) + (float) (intId & 1);
	    if (index <= id) {
		if (index == id) {
		    /* found it */
		    return stack + ({ node, mid * 2 + 1 });
		}
		high = mid;
	    } else {
		low = ++mid;
	    }
	}

	/* mid points at first entry higher than wanted */
	stack += ({ node, mid * 2 + 1 });
	branches = node->branches();
	if (sizeof(branches) <= mid || branches[mid] == 0) {
	    return stack;
	}
    }
}

/*
 * Get the next DatItem for the iterator.  The iterator state is a stack of
 * BTree nodes and offsets.  Walk depth-first through the BTrees and retrieve
 * DatItems in id-sorted order.
 */
mixed *iteratorNext(mixed stack)
{
    int size, offset, i;
    BTree node;
    int *branches;
    DatItem *entries;

    if (stack) {
	for (size = sizeof(stack); ; stack = stack[.. size - 1]) {
	    /*
	     * get current node
	     */
	    ({ node, offset }) = stack[size - 2 ..];
	    branches = node->branches();
	    entries = node->entries();
	    i = offset / 2;

	    if ((offset & 1) == 0) {	/* check branch */
		offset++;
		if (i < sizeof(branches) && branches[i] != 0) {
		    /*
		     * push branch node on the stack
		     */
		    stack[size - 1] = offset;
		    stack += ({ getBTree(branches[i]), 0 });
		    size += 2;
		    continue;
		}
	    }

	    if (i < sizeof(entries)) {
		if (stack[0] != nil && stack[0] < entries[i]->id()) {
		    return ({ nil, nil });
		}

		/*
		 * return DatItem entry
		 */
		stack[size - 1] = offset + 1;
		return ({ stack, entries[i] });
	    }

	    /*
	     * pop current node from the stack
	     */
	    size -= 2;
	    if (size == 1) {
		return ({ nil, nil });
	    }
	}
    }
}

/*
 * check whether the iterator has finished
 */
int iteratorEnd(mixed stack)
{
    return (!stack);
}
