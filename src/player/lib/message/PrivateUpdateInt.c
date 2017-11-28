# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateInt
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_INT, obj->getIntProperty(property));
}
