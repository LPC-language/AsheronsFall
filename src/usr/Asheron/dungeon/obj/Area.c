# include "Cell.h"
# include "dungeon.h"


mapping cells;

static void create()
{
    cells = ([ ]);
}

void setCell(int index, Cell cell)
{
    if (previous_program() == DUNGEONS) {
	cells[index] = cell;
    }
}

Cell cell(int index)
{
    if (previous_program() == DUNGEONS) {
	return cells[index];
    }
}
