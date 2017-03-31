# include "Cell.h"
# include "landblock.h"


object *grid;
string iteration;

static void create()
{
    compile_object(CELLBLOCK);
    compile_object(CELLINFO);
    compile_object(AREA);

    grid = allocate((255 / AREA_XDIM) * (255 / AREA_YDIM));
}

void setBlock(int x, int y, string landblock, int flags, int timeStamp,
	      int iteration)
{
    int gridX, gridY;
    Area area;

    gridX = x / AREA_XDIM;
    gridY = y / AREA_YDIM;
    area = grid[gridX + gridY * (255 / AREA_XDIM)];
    if (!area) {
	area = grid[gridX + gridY * (255 / AREA_XDIM)] =
	       clone_object(AREA, gridX * AREA_XDIM, gridY * AREA_YDIM);
    }
    area->setBlock(x, y, new Block(landblock, flags, timeStamp, iteration));
}

Cell block(int x, int y)
{
    return grid[x / AREA_XDIM + y / AREA_YDIM * (255 / AREA_XDIM)]->block(x, y);
}

void setInfo(int x, int y, string landinfo, int flags, int timeStamp,
	     int iteration)
{
    int gridX, gridY;
    Area area;

    gridX = x / AREA_XDIM;
    gridY = y / AREA_YDIM;
    area = grid[gridX + gridY * (255 / AREA_XDIM)];
    if (!area) {
	area = grid[gridX + gridY * (255 / AREA_XDIM)] =
	       clone_object(AREA, gridX * AREA_XDIM, gridY * AREA_YDIM);
    }
    area->setInfo(x, y, new Info(landinfo, flags, timeStamp, iteration));
}

Cell info(int x, int y)
{
    return grid[x / AREA_XDIM + y / AREA_YDIM * (255 / AREA_XDIM)]->info(x, y);
}

void setIterationData(string iteration)
{
    ::iteration = iteration;
}

string iteration() { return iteration; }
