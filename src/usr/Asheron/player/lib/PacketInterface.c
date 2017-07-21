# include <kernel/user.h>
# include "Interface.h"
# include "Account.h"
# include "Packet.h"
# include "RandSeq.h"

inherit Interface;

/*
 * ACK is sent when new seq received, at most once per 2 secs.  Timer resets
 * when retransmit request is sent.  When no further data included, ACK
 * reuses last seqno, checksum not encrypted.  Same for ReqRetrans with no
 * further data.
 *
 * Retransmit request is sent solo ~50 millisecs after missing packet detected;
 * depending on detected round-trip speed? Echo req/resp timing is as low as
 * 10 millisecs, so retrans req has a timer?  It also tries to hike on next
 * packet, like ACK.  2nd retrans req is sent after ~750 millisecs.
 * After receiving retransmission request, answer is sent immediately.
 *
 * Echo response can be delayed for a while, hikes on next packet?
 *
 * In portal-storm situations, it is the server which is losing input packets,
 * meaning that the input queue fills up faster than the server can digest
 * them, and packets are discarded.
 */


private Account account;		/* connected account */
private int serverId, clientId;		/* session IDs */
private RandSeq serverRand, clientRand;	/* for checksum encryption */
private int startTime;			/* session start time */
private int serverSeq, clientSeq;	/* sequence numbers */

/*
 * login during 3-way handshake, in case of an error return a packet to send
 */
static Packet login(string name, string password, int serverId, int clientId,
		    int serverSeed, int clientSeed)
{
    catch {
	account = ::login(name, password);
	if (!account) {
	    return nil;	/* XXX  bad password */
	}
    } : {
	return nil;	/* XXX login failed */
    }

    ::serverId = serverId;
    ::clientId = clientId;
    serverRand = new RandSeq(serverSeed);
    clientRand = new RandSeq(clientSeed);
    startTime = time();
    return nil;
}

/*
 * connection was established
 */
static void establish()
{
    serverSeq = clientSeq = 1;
    /* XXX reply with charlist */
}

/*
 * logout
 */
static void logout()
{
    ::logout(account);
}

/*
 * packet passed down by the higher layer
 */
static int receivePacket(string str)
{
    Packet packet;
    int sequence, flags;

    catch {
	packet = new ClientPacket(str, clientRand);
    } : {
	/* merely ignore bad packets, for now */
	return MODE_NOCHANGE;
    }

    /* handle incoming packet */
    sequence = packet->sequence();
    flags = packet->flags();
    /* XXX */
}


Account account()	{ return account; }
int clientId()		{ return clientId; }
