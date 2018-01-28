# include "Message.h"

inherit GenericMessage;


/*
 * create UpdateDouble
 */
static void create(object obj, int property)
{
    ::create(MSG_UPDATE_DOUBLE, serialize("i", obj->id()) +
				obj->getDoubleProperty(property));
}
