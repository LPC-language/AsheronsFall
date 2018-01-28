# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateVitalLevel
 */
static void create(object obj, int vital)
{
    ::create(MSG_PRIVATE_UPDATE_VITAL_LEVEL, obj->getVitalProperty(vital));
}
