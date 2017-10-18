# include <status.h>
# include <String.h>
# include "Serialized.h"

/*
 * a simple reader, for pieces smaller than half of one chunk
 */

private inherit Serialized;


/* 4 byte alignment */
# define ALIGN_DOWN(o)		((o) & ~3)
# define ALIGN_UP(o)		ALIGN_DOWN((o) + 3)

private string chunk1, chunk2;	/* first two chunks */
private int offset;		/* offset in first chunk */
private int limit;		/* half of maximum string length */
private StringBuffer buffer;	/* the StringBuffer to get more chunks from */

/*
 * initialize reader
 */
static void create(StringBuffer buffer)
{
    chunk1 = buffer->chunk();
    chunk2 = buffer->chunk();
    offset = 0;
    limit = status(ST_STRSIZE) / 2;
    ::buffer = buffer;
}

/*
 * remaining length
 */
int length()
{
    int length;

    length = strlen(chunk1) - offset + buffer->length();
    if (chunk2) {
	length += strlen(chunk2);
    }
    return length;
}

/*
 * refill the chunks
 */
private void refill()
{
    int boundary;

    boundary = ALIGN_DOWN(offset);
    if (chunk2 && strlen(chunk1) - boundary <= limit) {
	if (strlen(chunk2) <= limit) {
	    chunk1 = chunk1[boundary ..] + chunk2;
	    chunk2 = buffer->chunk();
	} else {
	    chunk1 = chunk1[boundary ..] + chunk2[.. limit - 1];
	    chunk2 = chunk2[limit ..];
	}
	offset -= boundary;
    }
}

/*
 * read items
 */
mixed *read(string format, varargs int n)
{
    mixed *list;

    list = deSerialize(chunk1, offset, format, n);
    offset = list[0];
    refill();
    return list[1 ..];
}

/*
 * skip bytes
 */
void skip(int bytes)
{
    offset += bytes;
    refill();
}

/*
 * 4 byte alignment
 */
void align()
{
    offset = ALIGN_UP(offset);
}
