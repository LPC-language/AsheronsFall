# include "File.h"
# include "Packet.h"
# include "pcap.h"


/*
 * Scanner for the collection of pcaps gathered in January 2017.  Scan
 * through all captures, save info in messages.dat.
 */

# define IP_NET_TURBINE		((198 << 16) + (252 << 8) + 160)

mapping captures;
mapping messages;

/*
 * initialize pcap scanner
 */
static void create()
{
    compile_object(OBJECT_PATH(Capture));
    compile_object(OBJECT_PATH(CaptureNG));
    compile_object(OBJECT_PATH(PacketEther));
    compile_object(OBJECT_PATH(PacketUDP));
    compile_object(OBJECT_PATH(File));

    compile_object(OBJECT_PATH(CapturedPacket));
    compile_object(OBJECT_PATH(CapturedServerSwitch));
    compile_object(OBJECT_PATH(CapturedReferral));
    compile_object(OBJECT_PATH(CapturedWorldLoginRequest));
    compile_object(OBJECT_PATH(CapturedConnectRequest));
    compile_object(OBJECT_PATH(CapturedEchoResponse));

    restore_object("~/pcap/data/captures.dat");
    call_out("walk", 0, map_indices(captures), 0);
    captures = nil;
    messages = ([ ]);
}

/*
 * capture walker
 */
static void walk(string *files, int index)
{
    string file;
    int len;
    object capture;
    mapping pcap;

    if (index < sizeof(files)) {
	file = files[index++];
	len = strlen(file);
	if (len >= 7 && file[len - 7 ..] == ".pcapng") {
	    /* pcapng */
	    capture = new CaptureNG("/usr/Asheron/pcap/data/" + file);
	} else {
	    /* pcap */
	    capture = new Capture("/usr/Asheron/pcap/data/" + file);
	}

	/*
	 * start processing a capture
	 */
	messages[file] = pcap = ([ ]);
	pcap["server"] = ([ ]);
	pcap["client"] = ([ ]);
	pcap["count"] = ([ ]);
	pcap["packets"] = 0;
	call_out("scan", 0, capture->iterator(), pcap, files, index);
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
 * mark messages
 */
private void markMessages(object packet, mapping pcap, mapping state)
{
    int sz, i, type;
    object fragment;
    string body;

    for (sz = packet->numFragments(), i = 0; i < sz; i++) {
	fragment = packet->fragment(i);
	if (fragment->index() == 0 && fragment->size() >= 18) {
	    body = fragment->body();
	    mark(body[0] + (body[1] << 8) + (fragment->group() << 16), pcap,
		 state);
	}
    }
}

/*
 * scan packets from a capture
 */
static void scan(object iter, mapping pcap, string *files, int index)
{
    object packet;
    int i;

    for (i = 0; i < 64; i++) {
	packet = nil;
	catch {
	    packet = iter->next();
	}
	if (packet) {
	    catch {
		packet = packet->packetUDP();
		if (packet) {
		    /*
		     * possible AC packet
		     */
		    if ((packet->srcAddr() >> 8) == IP_NET_TURBINE &&
			packet->srcPort() >= 1024) {
			/*
			 * server
			 */
			packet = packet->packetAC();
			if (packet->flags() & PACKET_BLOB_FRAGMENTS) {
			    markMessages(packet, pcap, pcap["server"]);
			}

		    } else if ((packet->destAddr() >> 8) == IP_NET_TURBINE &&
			       packet->destPort() >= 1024) {
			/*
			 * client
			 */
			packet = packet->packetAC();
			if ((packet->flags() & PACKET_BLOB_FRAGMENTS) &&
			    !(packet->flags() & PACKET_RETRANSMISSION)) {
			    markMessages(packet, pcap, pcap["client"]);
			}
		    }
		}
	    }
	    pcap["packets"]++;
	} else {
	    save_object("~/pcap/data/messages.dat");
	    walk(files, index);
	    return;
	}
    }

    call_out("scan", 0, iter, pcap, files, index);
}
