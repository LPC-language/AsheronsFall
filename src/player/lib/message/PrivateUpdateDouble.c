# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateUpdateDouble
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_DOUBLE, obj->getDoubleProperty(property));
}
