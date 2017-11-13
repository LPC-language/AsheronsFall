# include "Message.h"

inherit UpdateProperty;


/*
 * create UpdateData
 */
static void create(object obj, int property)
{
    ::create(MSG_UPDATE_DATA, serialize("i", obj->id()) +
			      obj->getDataProperties(({ property })));
}
