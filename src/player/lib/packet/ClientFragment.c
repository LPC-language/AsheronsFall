# include "Packet.h"

inherit Fragment;


# define HEADER_SIZE	16

/*
 * create from a blob
 */
static void create(string blob)
{
    string body;
    int sequence, id, count, size, index, group;

    ({
	body,
	sequence,
	id,
	count,
	size,
	index,
	group
    }) = deSerialize(blob, headerLayout());
    if (size > strlen(blob)) {
	error("Bad fragment size");
    }

    ::create(sequence, id, count, index, group, body[.. size - HEADER_SIZE - 1]);
}
