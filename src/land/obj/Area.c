# include "Cell.h"
# include "landblock.h"

int x, y;
Cell *blockGrid;
mapping infoMap;

static void create(int x, int y)
{
    ::x = x;
    ::y = y;
    blockGrid = allocate(AREA_XDIM * AREA_YDIM);
    infoMap = ([ ]);
}

void setBlock(int x, int y, Cell landblock)
{
    if (previous_program() == LANDBLOCKS) {
	blockGrid[x - ::x + (y - ::y) * AREA_XDIM] = landblock;
    }
}

void setInfo(int x, int y, Cell info)
{
    if (previous_program() == LANDBLOCKS) {
	infoMap[x + y * AREA_XDIM] = info;
    }
}

Cell block(int x, int y)
{
    if (previous_program() == LANDBLOCKS) {
	return blockGrid[x - ::x + (y - ::y) * AREA_XDIM];
    }
}

Cell info(int x, int y)
{
    if (previous_program() == LANDBLOCKS) {
	return infoMap[x + y * AREA_XDIM];
    }
}
