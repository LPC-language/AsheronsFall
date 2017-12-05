# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateAttribute
 */
static void create(object obj, int attr)
{
    ::create(MSG_PRIVATE_UPDATE_ATTRIBUTE,
	     serialize("i", attr) + obj->getAttribute(attr));
}