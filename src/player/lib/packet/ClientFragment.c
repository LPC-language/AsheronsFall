# include "Packet.h"

inherit Fragment;


# define HEADER_SIZE	16

/*
 * create from a blob
 */
static void create(string blob, int offset)
{
    int sequence, id, count, size, index, group;

    ({
	offset,
	sequence,
	id,
	count,
	size,
	index,
	group
    }) = deSerialize(blob, offset, headerLayout());
    size -= HEADER_SIZE;
    if (size <= 0 || offset + size > strlen(blob)) {
	error("Bad fragment size");
    }

    ::create(sequence, id, count, index, group,
	     blob[offset .. offset + size - 1]);
}
