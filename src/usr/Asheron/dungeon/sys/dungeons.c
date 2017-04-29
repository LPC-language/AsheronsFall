# include "Cell.h"
# include "dungeon.h"


mapping dungeons;

static void create()
{
    compile_object(OBJECT_PATH(DungeonCell));
    compile_object(OBJECT_PATH(DungeonArea));

    dungeons = ([ ]);
}

void setCell(int x, int y, int index, string cell, int flags, int timeStamp,
	     int iteration)
{
    DungeonArea area;

    area = dungeons[x + y * 255];
    if (!area) {
	area = dungeons[x + y * 255] =
	       clone_object(OBJECT_PATH(DungeonArea));
    }
    area->setCell(index, new DungeonCell(cell, flags, timeStamp, iteration));
}

Cell cell(int x, int y, int index)
{
    return dungeons[x + y * 255]->cell(index);
}
