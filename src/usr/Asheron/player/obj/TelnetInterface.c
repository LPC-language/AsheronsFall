# include <kernel/user.h>
# include "Interface.h"

inherit Interface;

int login(string str)
{
    if (previous_program() == LIB_CONN) {
	connection(previous_object());
	::login(str);
    }
}
