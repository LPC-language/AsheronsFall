private string data;
private int flags;
private int timeStamp;
private int iteration;

static void create(string data, int flags, int timeStamp, int iteration)
{
    ::data = data;
    ::flags = flags;
    ::timeStamp = timeStamp;
    ::iteration = iteration;
}

string data()	{ return data; }
int flags()	{ return flags; }
int timeStamp()	{ return timeStamp; }
int iteration()	{ return iteration; }
