# include "Object.h"
# include "User.h"

inherit Object;


Account account;
string name;

static void create(Account account, string name)
{
    ::create(persistentId(), 0);
    ::name = name;
}


Account account()	{ return account; }
string name()		{ return name; }
int deleteTimer()	{ return 0; }
