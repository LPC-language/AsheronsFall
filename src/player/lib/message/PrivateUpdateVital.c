# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateUpdateVital
 */
static void create(object obj, int vital)
{
    ::create(MSG_PRIVATE_UPDATE_VITAL, obj->getVitalAttribute(vital));
}
