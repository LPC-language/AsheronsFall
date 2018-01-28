# include "Message.h"

inherit GenericMessage;


/*
 * create UpdateString
 */
static void create(object obj, int property)
{
    string body;

    /*
     * note the order: sequence, property, objectId, value
     */
    body = obj->getStringProperty(property);
    ::create(MSG_UPDATE_STRING, body[.. 4] + serialize("i", obj->id()) +
				"\0\0\0" + body[5 ..]);
}
