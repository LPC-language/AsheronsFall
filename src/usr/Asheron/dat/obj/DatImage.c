# include <String.h>
# include <Iterator.h>
# include "Serialized.h"
# include "BTree.h"
# include "Dat.h"

inherit Iterable;
inherit Serialized;


string fileName;	/* name of the DAT file */
BTree root;		/* btree of DAT items */

# define HEADER_OFFSET	320
# define FILE_TYPE	0x5442
# define FREE_BLOCK	0x80000000

int fileType;		/* 'TB' */
int blockSize;		/* size of a block */
int fileSize;		/* size of the DAT file */
int dataSet;
int dataSubset;
int freeHead;
int freeTail;
int freeCount;
int blockBTree;		/* block of BTree root node */

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

    /* some minimal sanity checks */
    if (fileType != FILE_TYPE) {
	error("Not a DAT file");
    }
    if (fileSize != file_info(fileName)[0]) {
	error("Invalid length in DAT header");
    }
}

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

private StringBuffer getData(int block, int length)
{
    StringBuffer buffer;
    int size, i, len;
    string chunk;

    buffer = new StringBuffer;
    size = (length - 1) / (blockSize - 4) + 1;
    for (i = 0; i < size; i++) {
	chunk = getBlock(block);
	({ block }) = deSerialize(chunk, "i");
	len = (length < blockSize) ? length : blockSize;
	buffer->append(chunk[4 .. len - 1]);
    }

    return buffer;
}

private BTree getBTree(int block)
{
    return new BTree(getData(block, BTREE_SIZE)->chunk());
}

private DatItem getDatItem(int *entry)
{
    int flags, id, block, length, timeStamp, iteration;

    ({ flags, id, block, length, timeStamp, iteration }) = entry;
    return new DatItem(flags, id, timeStamp, iteration,
		       new String(getData(block, length)));
}

static void create(string file)
{
    fileName = file;

    loadHeader();
    root = getBTree(blockBTree);
}

string fileName() { return fileName; }

mixed iteratorStart(mixed stack)
{
    return ({ root, 0 });
}

mixed *iteratorNext(mixed stack)
{
    int size, offset, i;
    BTree *node;
    int *branches, **entries;

    if (stack[0]) {
	size = sizeof(stack);
	for (;;) {
	    ({ node, offset }) = stack[size - 2 ..];
	    branches = node->branches();
	    entries = node->entries();
	    i = offset / 2;

	    if ((offset & 1) == 0) {
		/*
		 * check branch first
		 */
		offset++;
		if (i < sizeof(branches) && branches[i] != 0) {
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

	    size -= 2;
	    if (size == 0) {
		break;
	    }
	    stack = stack[.. size - 1];
	}
    }

    return ({ nil, nil });
}

int iteratorEnd(mixed stack)
{
    return (!stack[0]);
}

Iterator iterator()
{
    return new Iterator(this_object(), iteratorStart(nil));
}
