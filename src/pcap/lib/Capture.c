# include <Iterator.h>
# include <Time.h>
# include "Serialized.h"
# include "File.h"
# include "pcap.h"

inherit Serialized;
inherit Iterable;


# define PCAP_HEADER_SIZE	24
# define PCAP_MAGIC		0xa1b2c3d4
# define PACKET_HEADER_SIZE	16

# define ETHER_HEADER_SIZE	14
# define IP_HEADER_SIZE		20
# define IP_MAX_OPTIONS_SIZE	40
# define UDP_HEADER_SIZE	8
# define AC_MAX_PACKET_SIZE	484

private string fileName;	/* pcap filename */
private int fileSize;		/* size of pcap file */
private File file;		/* file to read from */
private int versionMajor;	/* major version number */
private int versionMinor;	/* minor version number */
private int timezone;		/* always 0 */
private int snapLength;		/* should be large enough */
private int network;		/* data link type */

/*
 * initialize a PCAP capture object, which can be used to iterate over packets
 */
static void create(string pcap)
{
    string header;
    int offset, magic, sigfigs;

    fileName = pcap;
    fileSize = file_info(pcap)[0];
    file = new File(pcap);
    header = file->read(0, PCAP_HEADER_SIZE);
    ({
	offset,
	magic,
	versionMajor,
	versionMinor,
	timezone,
	sigfigs,
	snapLength,
	network
    }) = deSerialize(header, 0, "issiiii");
    if (magic != PCAP_MAGIC) {
	error("Not a PCAP file");
    }
    if (snapLength < ETHER_HEADER_SIZE + IP_HEADER_SIZE + IP_MAX_OPTIONS_SIZE +
		     UDP_HEADER_SIZE + AC_MAX_PACKET_SIZE) {
	error("PCAP snap length too small for AC packets");
    }
}

/*
 * initial iterator state
 */
mixed iteratorStart(mixed from, mixed to)
{
    if (from != nil || to != nil) {
	error("Subrange not implemented");
    }
    return PCAP_HEADER_SIZE;
}

/*
 * obtain the next network packet from the packet capture
 */
mixed *iteratorNext(mixed offset)
{
    string header;
    int dummy, time, utime, length, origLength;

    if (offset == fileSize) {
	return ({ offset, nil });	/* end of file */
    }

    header = file->read(offset, PACKET_HEADER_SIZE);
    offset += PACKET_HEADER_SIZE;
    ({
	dummy,
	time,
	utime,
	length,
	origLength
    }) = deSerialize(header, 0, "iiii");
    if (length > snapLength || offset + length > fileSize) {
	error("Invalid PCAP record");
    }

    return ({
	offset + length,
	new PacketEther(new Time(time, (float) utime / 1000000.0), origLength,
			file->read(offset, length))
   });
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
int snapLength()	{ return snapLength; }
int network()		{ return network; }
