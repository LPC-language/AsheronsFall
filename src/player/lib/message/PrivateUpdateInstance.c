# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateInstance
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_INSTANCE,
	     obj->getInstanceProperties(({ property })));
}
