# include <kernel/user.h>
# include "Interface.h"
# include "Packet.h"

inherit Interface;


int n;

int login(string str)
{
    if (previous_program() == LIB_CONN) {
	write_file("/usr/Asheron/data/" + n++, str);
	/* ::login(previous_object(), str); */
    }
    return MODE_NOCHANGE;
}

void logout()
{
    if (previous_program() == LIB_CONN) {
	::logout();
    }
}


int receive_message(string str)
{
    if (previous_program() == LIB_CONN) {
	write_file("/usr/Asheron/data/" + n++, str);
    }
    return MODE_NOCHANGE;
}
