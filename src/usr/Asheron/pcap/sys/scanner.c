# include "pcap.h"
# include "File.h"
# include "Packet.h"
# include "RandSeq.h"


/*
 * Scanner for the collection of pcaps gathered in January 2017.  Scan
 * through all captures, save info in scanner.dat.
 */

# define IP_NET_TURBINE		((198 << 16) + (252 << 8) + 160)

mapping captures;

/*
 * initialize pcap scanner
 */
static void create()
{
    compile_object(OBJECT_PATH(Capture));
    compile_object(OBJECT_PATH(CaptureNG));
    compile_object(OBJECT_PATH(PacketEther));
    compile_object(OBJECT_PATH(PacketAC));
    compile_object(OBJECT_PATH(File));
    compile_object(OBJECT_PATH(FileTree));

    compile_object(OBJECT_PATH(CapturedPacket));
    compile_object(OBJECT_PATH(ClientServerSwitch));
    compile_object(OBJECT_PATH(ClientReferral));
    compile_object(OBJECT_PATH(ClientWorldLoginRequest));
    compile_object(OBJECT_PATH(ClientConnectRequest));
    compile_object(OBJECT_PATH(ClientConnectClose));
    compile_object(OBJECT_PATH(ClientEchoResponse));

    captures = ([ ]);
    call_out("walk", 0, new FileTree("/usr/Asheron/pcap/data")->iterator());
}

/*
 * find a description file in the same location
 */
private string getDescFile(string file)
{
    int len;
    string desc;

    len = strlen(file);
    desc = read_file(file + ".txt");
    if (!desc && len >= 4 && file[len - 4 ..] == "_log") {
	desc = read_file(file[ .. len - 5] + "_desc.txt");
    }

    if (desc && strlen(desc) >= 10 && desc[3] == 0x58 && desc[7] == 0x00) {
	/* skip 10 byte header */
	desc = desc[10 ..];
    }
    return desc;
}

/*
 * find a description file in the same location, or one directory up
 */
private string getDescPath(string file)
{
    string desc, *path;
    int size;

    desc = getDescFile(file);
    if (!desc) {
	/*
	 * sometimes pcap logs are in their own subdirectory
	 */
	path = explode(file, "/");
	size = sizeof(path);
	desc = getDescFile("/" + implode(path[.. size - 3] + path[size - 1 ..],
					 "/"));
    }
    return desc;
}

/*
 * file tree walker
 */
static void walk(object walker)
{
    string file, base;
    int len;
    object capture;
    mapping pcap;

    file = walker->next();
    if (file != nil) {
	len = strlen(file);
	if (len >= 5 && file[len - 5 ..] == ".pcap") {
	    /* pcap */
	    base = file[.. len - 6];
	    capture = new Capture(file);
	} else if (len >= 7 && file[len - 7 ..] == ".pcapng") {
	    /* pcapng */
	    base = file[.. len - 8];
	    capture = new CaptureNG(file);
	} else {
	    /* not a capture */
	    call_out("walk", 0, walker);
	    return;
	}

	/*
	 * start processing a capture
	 */
	captures[file] = pcap = ([ ]);
	pcap["description"] = getDescPath(base);
	pcap["server"] = ([ ]);
	pcap["client"] = ([ ]);
	pcap["count"] = ([ ]);
	pcap["packets"] = 0;
	pcap["nonACPackets"] = 0;
	pcap["servers"] = ({ });
	call_out("scan", 0, capture->iterator(), pcap, ([ ]), walker);
    }
}

/*
 * mark something in the record
 */
private void mark(mixed index, mapping pcap, mapping state)
{
    int n;
    mapping count;

    n = pcap["packets"];
    count = pcap["count"];

    /* mark first occurrance */
    if (!state[index]) {
	state[index] = n;
    }
    /* count all occurrances */
    if (!count[index]) {
	count[index] = 1;
    } else {
	count[index]++;
    }
}

/*
 * mark flags
 */
private void markFlags(object packet, mapping pcap, mapping state)
{
    int flags, i;

    flags = packet->flags();
    for (i = 0; i < 28; i++) {
	if (flags & (1 << i)) {
	    mark(i, pcap, state);
	}
    }
}

/*
 * mark checksum
 */
private void markChecksum(Packet packet, object rand, mapping pcap,
			  mapping state)
{
    catch {
	if (packet->flags() & PACKET_ENCRYPTED_CHECKSUM) {
	    if (rand) {
		packet->addXorValue(rand->rand(packet->sequence() - 2));
		if (packet->checksum() != packet->computeChecksum()) {
		    mark("badEncryptedChecksum", pcap, state);
		}
	    } else {
		mark("noSeedChecksum", pcap, state);
	    }
	} else if (packet->checksum() != packet->computeChecksum()) {
	    mark("badChecksum", pcap, state);
	}
    } : {
	mark("errorChecksum", pcap, state);
    }
}

/*
 * scan packets from a capture
 */
static void scan(object iter, mapping pcap, mapping rand, object walker)
{
    object packet;
    int addrPort;

    catch {
	packet = iter->next();
    } : {
	pcap["errorCapture"] = 1;
    }
    if (packet) {
	catch {
	    packet = packet->packetAC();
	    if (packet) {
		if (pcap["packets"] == 0) {
		    pcap["startTime"] = packet->time();
		}
		pcap["endTime"] = packet->time();

		if ((packet->srcPort() >= 9000 && packet->srcPort() <= 9099) ||
		    (packet->destPort() >= 9000 && packet->destPort() <= 9099))
		{
		    /*
		     * AC-like
		     */
		    if ((packet->srcAddr() >> 8) == IP_NET_TURBINE) {
			/*
			 * server
			 */
			pcap["servers"] |= ({ packet->srcAddr() & 0xff });
			addrPort = ((packet->srcAddr() & 0xff) << 16) +
				   packet->srcPort();
			packet = packet->packet();
			markFlags(packet, pcap, pcap["server"]);
			markChecksum(packet, rand[addrPort], pcap,
				     pcap["server"]);

			if (packet->flags() & PACKET_CONNECT_REQUEST) {
			    packet = packet->data(PACKET_CONNECT_REQUEST);
			    rand[addrPort] = new RandSeq(packet->clientSeed());
			    rand[addrPort + 1] = new RandSeq(packet->serverSeed());
			}
		    } else if ((packet->destAddr() >> 8) == IP_NET_TURBINE) {
			/*
			 * client
			 */
			pcap["servers"] |= ({ packet->destAddr() & 0xff });
			addrPort = ((packet->destAddr() & 0xff) << 16) +
				   packet->destPort();
			packet = packet->packet();
			markFlags(packet, pcap, pcap["client"]);
			markChecksum(packet, rand[addrPort], pcap,
				     pcap["client"]);
		    } else {
			/*
			 * not an AC packet after all
			 */
			pcap["nonACPackets"]++;
		    }
		} else {
		    pcap["nonACPackets"]++;
		}
	    }
	} : {
	    mark("errorPackets", pcap, pcap);
	}
	pcap["packets"]++;
	call_out("scan", 0, iter, pcap, rand, walker);
    } else {
	save_object("~/pcap/sys/scanner.dat");
	walk(walker);
    }
}
