# include <status.h>
# include <String.h>
# include "Serialized.h"

/*
 * a simple reader, for pieces smaller than half of one chunk
 */

private inherit Serialized;


private string chunk1, chunk2;	/* first two chunks */
private int limit;		/* half of maximum string length */
private StringBuffer buffer;	/* the StringBuffer to get more chunks from */

/*
 * initialize reader
 */
static void create(StringBuffer buffer)
{
    chunk1 = buffer->chunk();
    chunk2 = buffer->chunk();
    limit = status(ST_STRSIZE) / 2;
    ::buffer = buffer;
}

/*
 * remaining length
 */
int length()
{
    int length;

    length = strlen(chunk1) + buffer->length();
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
    if (chunk2 && strlen(chunk1) <= limit) {
	if (strlen(chunk2) <= limit) {
	    chunk1 += chunk2;
	    chunk2 = buffer->chunk();
	} else {
	    chunk1 += chunk2[.. limit - 1];
	    chunk2 = chunk2[limit ..];
	}
    }
}

/*
 * read items
 */
mixed *read(string format, varargs int n)
{
    mixed *list;

    list = deSerialize(chunk1, format, n);
    chunk1 = list[0];
    refill();
    return list[1 ..];
}

/*
 * skip bytes
 */
void skip(int bytes)
{
    chunk1 = chunk1[bytes ..];
    refill();
}
