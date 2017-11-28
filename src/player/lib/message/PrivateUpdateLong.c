# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateLong
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_LONG, obj->getLongProperty(property));
}
