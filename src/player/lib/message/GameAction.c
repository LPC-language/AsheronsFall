# include "Message.h"

inherit Message;


private int actionType;		/* type of GameAction */

/*
 * create GameAction
 */
static void create(int actionType)
{
    ::create(MSG_GAME_ACTION);
    ::actionType = actionType;
}


int actionType()		{ return actionType; }
