# include "GameEvent.h"

inherit GameEvent;


static void create(object player, int *displayTitles, int *titleList)
{
    ::create(EVT_CHARACTER_TITLE, player,
	     serializeArray(displayTitles) + serializeArray(titleList));
}
