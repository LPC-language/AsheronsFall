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
    ({
	fileType,
	blockSize,
	fileSize,
	dataSet,
	dataSubset,
	freeHead,
	freeTail,
	freeCount,
	blockBTree
    }) = deSerialize(read_file(fileName, HEADER_OFFSET, 36), "iiiiiiiii");

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
    int n;
    string chunk;

    buffer = new StringBuffer;
    if (length != 0) {
	for (n = (length - 1) / (blockSize - 4); n != 0; --n) {
	    chunk = getBlock(block);
	    ({ block }) = deSerialize(chunk, "i");
	    buffer->append(chunk[4 ..]);
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
 * given a BTree entry, load an item from the DAT image
 */
private DatItem getDatItem(int *entry)
{
    int flags, id, block, length, timeStamp, iteration;

    ({ flags, id, block, length, timeStamp, iteration }) = entry;
    return new DatItem(flags, id, timeStamp, iteration, getData(block, length));
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
mixed iteratorStart(mixed stack)
{
    return ({ root, 0 });
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
    int *branches, **entries;

    if (stack) {
	size = sizeof(stack);
	for (;;) {
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
		/*
		 * get DatItem from entry
		 */
		stack[size - 1] = offset + 1;
		return ({ stack, getDatItem(entries[i]) });
	    }

	    /*
	     * pop current node from the stack
	     */
	    size -= 2;
	    if (size == 0) {
		break;
	    }
	    stack = stack[.. size - 1];
	}
    }

    return ({ nil, nil });
}

/*
 * check whether the iterator is finished
 */
int iteratorEnd(mixed stack)
{
    return (!stack);
}
