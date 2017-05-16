# include "Interface.h"
# include "Packet.h"
# include "RandSeq.h"

inherit Interface;

/*
 * maintain state, queues
 * handle ISAAC encryption
 */


private RandSeq serverRand, clientRand;
private int serverSeq, clientSeq;

static void login(string name, int serverSeed, int clientSeed)
{
    ::login(name);
    serverRand = new RandSeq(serverSeed);
    clientRand = new RandSeq(clientRand);
}

int randSequence(int sequence)
{
    return clientRand->rand(0);
}
