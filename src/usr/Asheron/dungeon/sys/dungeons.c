# include "Cell.h"
# include "dungeon.h"


mapping dungeons;

static void create()
{
    compile_object(DUNGEONCELL);
    compile_object(DUNGEONAREA);

    dungeons = ([ ]);
}

void setCell(int x, int y, int index, string cell, int flags, int timeStamp,
	     int iteration)
{
    DungeonArea area;

    area = dungeons[x + y * 255];
    if (!area) {
	area = dungeons[x + y * 255] =
	       clone_object(DUNGEONAREA);
    }
    area->setCell(index, new DUNGEONCELL(cell, flags, timeStamp, iteration));
}

Cell cell(int x, int y, int index)
{
    return dungeons[x + y * 255]->cell(index);
}
