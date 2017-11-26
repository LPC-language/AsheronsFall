# include "Message.h"

inherit UpdateProperty;


/*
 * create PrivateUpdateSkill
 */
static void create(object obj, int skill)
{
    ::create(MSG_PRIVATE_UPDATE_SKILL, obj->getSkill(skill));
}
