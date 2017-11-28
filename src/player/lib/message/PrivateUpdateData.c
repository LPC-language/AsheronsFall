# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateData
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_DATA, obj->getDataProperty(property));
}
