# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateRemoveInstance
 */
static void create(int property)
{
    ::create(MSG_PRIVATE_REMOVE_INSTANCE, serialize("i", property));
}
