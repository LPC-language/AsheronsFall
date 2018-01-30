# include "Object.h"

inherit PhysicalObject;


private string name;

string transport()
{
    return ::transport() +
	   "\x16\x00\x80\x00" + serialize("t", name) +
	   "\x01\x00\x36\x10\x10\x00\x00\x00\x1C\x00\x00\x00\x66\x07\x01\x00" +
	   "\x00\x00\x04\x00";
}

static void create(int class, int id, string name)
{
    ::create(class, id);
    ::name = name;
}


string name()	{ return name; }
