# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateDouble
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_DOUBLE, obj->getDoubleProperty(property));
}
