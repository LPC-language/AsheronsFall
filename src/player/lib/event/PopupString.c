# include "GameEvent.h"

inherit GameEvent;


static void create(object player, string str)
{
    ::create(EVT_POPUP_STRING, player, serialize("t", str));
}
