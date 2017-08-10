# include "pcap.h"
# include "Serialized.h"
# include "File.h"
# include <Iterator.h>

inherit Serialized;
inherit Iterable;


# define PCAPNG_HEADER_SIZE	16
# define PCAPNG_MAGIC		0x0a0d0d0a
# define PCAPNG_ORDER_MAGIC	0x1a2b3c4d
# define PCAPNG_IFACE_DESC	1
# define PCAPNG_SIMPLE_PACKET	3
# define PCAPNG_ENHANCED_PACKET	6
# define PACKET_HEADER_SIZE	12
# define PACKET_ENHANCED_SIZE	16

# define ETHER_HEADER_SIZE	14
# define IP_HEADER_SIZE		20
# define IP_MAX_OPTIONS_SIZE	40
# define UDP_HEADER_SIZE	8
# define AC_MAX_PACKET_SIZE	484

private string fileName;	/* pcapng file name */
private int fileSize;		/* size of pcapng file */
private File file;		/* file to read from */
private int versionMajor;	/* major version number */
private int versionMinor;	/* minor version number */
private int start;		/* offset in file after header */

/*
 * initialize a PCAPNG capture object, which can be used to iterate over packets
 */
static void create(string pcapng)
{
    string header;
    int magic, orderMagic;

    file = new File(pcapng);
    header = file->read(0, PCAPNG_HEADER_SIZE);
    ({
	header,
	magic,
	start,
	orderMagic,
	versionMajor,
	versionMinor
    }) = deSerialize(header, "iiiss");
    if (magic != PCAPNG_MAGIC) {
	error("Not a PCAPNG file");
    }
    if (orderMagic != PCAPNG_ORDER_MAGIC) {
	error("Not a little endian PCAPNG file");
    }
    fileSize = file_info(pcapng)[0];
}

/*
 * initial iterator state
 */
mixed iteratorStart(mixed state)
{
    return start;
}

/*
 * obtain the next network packet from the packet capture
 */
mixed *iteratorNext(mixed offset)
{
    string header;
    int type, length, origLength, snapLength, timeHigh, timeLow;
    int time, part, low;
    float mtime, high;

    while (offset < fileSize) {
	header = file->read(offset, PACKET_HEADER_SIZE);
	({
	    header,
	    type,
	    length,
	    origLength
	}) = deSerialize(header, "iii");
	if (offset + length > fileSize) {
	    error("Invalid PCAPNG record");
	}

	switch (type) {
	case PCAPNG_IFACE_DESC:
	    header = file->read(offset + PACKET_HEADER_SIZE, 4);
	    ({
		header,
		snapLength
	    }) = deSerialize(header, "i");
	    if (snapLength < ETHER_HEADER_SIZE + IP_HEADER_SIZE +
			     IP_MAX_OPTIONS_SIZE + UDP_HEADER_SIZE +
			     AC_MAX_PACKET_SIZE) {
		error("PCAPNG snap length too small for AC packets");
	    }
	    offset += length;
	    break;

	case PCAPNG_SIMPLE_PACKET:
	    return ({
		offset + length,
		new PacketEther(0, 0.0, origLength,
				file->read(offset + PACKET_HEADER_SIZE,
					   origLength))
	   });

	case PCAPNG_ENHANCED_PACKET:
	    header = file->read(offset + PACKET_HEADER_SIZE,
				PACKET_ENHANCED_SIZE);
	    ({
		header,
		timeHigh,
		timeLow,
		snapLength,
		origLength
	    }) = deSerialize(header, "iiii");

	    /* carefully add the bits without losing precision */
	    time = timeHigh * 4294;
	    part = timeHigh * 967;
	    time += part / 1000;
	    low = part % 1000 * 1000;
	    part = timeHigh * 296;
	    time += part / 1000000;
	    low += part % 1000000;
	    mtime = (float) low + (float) (timeLow >> 1) * 2.0 +
		    (float) (timeLow & 1);
	    ({ mtime, high }) = modf(mtime / 1000000.0);
	    time += (int) high;

	    return ({
		offset + length,
		new PacketEther(time, mtime, origLength,
				file->read(offset + PACKET_HEADER_SIZE +
							PACKET_ENHANCED_SIZE,
					   snapLength))
	    });

	default:
	    offset += length;
	    break;
	}
    }

    return ({ offset, nil });	/* end of file */
}

/*
 * check if another packet can be retrieved
 */
int iteratorEnd(mixed offset)
{
    return (offset == fileSize);
}


string fileName()	{ return fileName; }
int versionMajor()	{ return versionMajor; }
int versionMinor()	{ return versionMinor; }
