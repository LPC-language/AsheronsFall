# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateUpdateLong
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_LONG, obj->getLongProperty(property));
}
