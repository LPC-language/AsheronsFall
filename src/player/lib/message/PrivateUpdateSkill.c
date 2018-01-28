# include "Message.h"

inherit GenericMessage;


/*
 * create PrivateUpdateSkill
 */
static void create(object obj, int skill)
{
    ::create(MSG_PRIVATE_UPDATE_SKILL, obj->getSkillProperty(skill));
}
