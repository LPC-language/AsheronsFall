# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateUpdatePosition
 */
static void create(object obj, int property)
{
    ::create(MSG_PRIVATE_UPDATE_POSITION, obj->getPositionPropertiy(property));
}
