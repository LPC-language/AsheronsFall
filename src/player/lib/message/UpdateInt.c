# include "Message.h"

inherit GenericMessage;


/*
 * create UpdateInt
 */
static void create(object obj, int property)
{
    ::create(MSG_UPDATE_INT, serialize("i", obj->id()) +
			     obj->getIntProperty(property));
}
