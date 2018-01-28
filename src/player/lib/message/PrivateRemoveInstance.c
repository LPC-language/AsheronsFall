# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateRemoveInstance
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_REMOVE_INSTANCE,
	     obj->removeInstanceProperty(property));
}
