# include "GameEvent.h"
# include "statusmessage.h"

inherit GameEvent;


static void create(object player, int message)
{
    ::create(EVT_STATUS_MESSAGE, player, serialize("i", message));
}
