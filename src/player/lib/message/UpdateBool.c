# include "Message.h"

inherit UpdateProperty;


/*
 * create UpdateBool
 */
static void create(object obj, int property)
{
    ::create(MSG_UPDATE_BOOL, serialize("i", obj->id()) +
			      obj->getBoolProperties(({ property })));
}
