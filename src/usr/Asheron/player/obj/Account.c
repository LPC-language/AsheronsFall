# include "Interface.h"

string name;
string password;
Interface interface;

static void create(string name)
{
    ::name = name;
}

void setPassword(string str)
{
    /* XXX security check */
    password = hash_string("crypt", str);
}

int checkPassword(string str)
{
    /* XXX security check */
    return (hash_string("crypt", str, password) == password);
}
