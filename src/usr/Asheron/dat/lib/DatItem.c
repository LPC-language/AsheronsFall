# include <String.h>

private int flags;
private int id;
private int timeStamp;
private int iteration;
private String data;

static void create(int flags, int id, int timeStamp, int iteration, String str)
{
    ::flags = flags;
    ::id = id;
    ::timeStamp = timeStamp;
    ::iteration = iteration;
    ::data = str;
}

int flags()	{ return flags; }
int id()	{ return id; }
int timeStamp()	{ return timeStamp; }
int iteration()	{ return iteration; }
String data()	{ return data; }
