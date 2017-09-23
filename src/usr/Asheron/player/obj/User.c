# include "User.h"
# include "Interface.h"

Account account;
Interface interface;

static void create(Account account, Interface interface)
{
    ::account = account;
    ::interface = interface;
}

void logout()
{
    if (previous_object() == interface) {
	destruct_object(this_object());
    }
}
