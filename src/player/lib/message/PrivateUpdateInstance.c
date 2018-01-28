# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateUpdateInstance
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_INSTANCE, obj->getInstanceProperty(property));
}
