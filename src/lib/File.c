# define BLOCK_SIZE	8192

private string file;		/* the file to read */
private int fileSize;		/* size of that file */
private string cached;		/* last block read */
private int cachedOffset;	/* last block offset */

/*
 * initialize file reader
 */
static void create(string file)
{
    mixed *info;

    info = file_info(file);
    if (!info) {
	error("No such file");
    }

    ::file = file;
    fileSize = info[0];
    cachedOffset = -1;
}

/*
 * get a block
 */
private string getBlock(int offset, int size)
{
    if (offset != cachedOffset) {
	cached = read_file(file, offset, size);
	cachedOffset = offset;
    }
    return cached;
}

/*
 * get a chunk, of which the first and last blocks will be retrieved through
 * the cache
 */
string read(int offset, int size)
{
    int endOffset, endSize;
    string chunk;

    endOffset = offset + size - 1;
    if (endOffset >= fileSize) {
	error("Read beyond end of file");
    }
    if (size == 0) {
	return "";
    }
    endOffset &= ~(BLOCK_SIZE - 1);
    endSize = (fileSize - endOffset >= BLOCK_SIZE) ?
	       BLOCK_SIZE : fileSize - endOffset;

    if (endOffset > offset) {
	int startOffset;
	string startBlock, midBlock;

	startOffset = offset & ~(BLOCK_SIZE - 1);
	startBlock = getBlock(startOffset, BLOCK_SIZE);

	if (endOffset > startOffset + BLOCK_SIZE) {
	    startOffset += BLOCK_SIZE;
	    midBlock = read_file(file, startOffset, endOffset - startOffset);
	} else {
	    midBlock = "";
	}

	chunk = startBlock + midBlock + getBlock(endOffset, endSize);
    } else {
	chunk = getBlock(endOffset, endSize);
    }

    offset = offset & (BLOCK_SIZE - 1);
    return chunk[offset .. offset + size - 1];
}
