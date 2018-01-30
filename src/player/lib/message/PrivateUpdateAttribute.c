# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateUpdateAttribute
 */
static void create(object obj, int attr)
{
    ::create(MSG_PRIVATE_UPDATE_ATTRIBUTE, obj->getAttribute(attr));
}
