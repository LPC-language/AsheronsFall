# include "Message.h"

inherit UpdateProperty;


/*
 * create UpdateString
 */
static void create(object obj, int property)
{
    string body;

    /*
     * note the order: property, objectId, value
     */
    body = obj->getStringProperty(property);
    ::create(MSG_UPDATE_STRING, body[.. 3] + serialize("i", obj->id()) +
				"\0\0\0" + body[4 ..]);
}
