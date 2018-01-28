# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateRemoveInstance
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_REMOVE_INSTANCE,
	     obj->removeInstanceProperty(property));
}
