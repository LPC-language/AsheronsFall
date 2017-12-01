# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateVital
 */
static void create(object obj, int vital)
{
    ::create(MSG_PRIVATE_UPDATE_VITAL,
	     serialize("i", vital) + obj->getVitalAttribute(vital));
}
