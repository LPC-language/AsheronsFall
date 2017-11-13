# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdatePosition
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_POSITION,
	     obj->getPositionProperties(({ property })));
}
