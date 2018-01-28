# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateUpdateInt
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_INT, obj->getIntProperty(property));
}
