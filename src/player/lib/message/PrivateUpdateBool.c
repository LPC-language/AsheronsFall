# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateBool
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_BOOL, obj->getBoolProperty(property));
}
