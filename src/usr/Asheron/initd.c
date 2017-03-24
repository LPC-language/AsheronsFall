# include <Iterator.h>
# include "BTree.h"
# include "Dat.h"
# include "landblock.h"
# include "dungeon.h"


object cell_1;
object portal;
object highres;
object english;

object land;
object dungeons;

static void create()
{
    compile_object(BTREE);
    compile_object(DATITEM);
    compile_object(DATIMAGE);
    cell_1 = clone_object(DATIMAGE, "/usr/Asheron/dat/data/client_cell_1.dat");
    portal = clone_object(DATIMAGE, "/usr/Asheron/dat/data/client_portal.dat");
    highres = clone_object(DATIMAGE,
			   "/usr/Asheron/dat/data/client_highres.dat");
    english = clone_object(DATIMAGE,
			   "/usr/Asheron/dat/data/client_local_English.dat");

    land = compile_object(LANDBLOCKS);
    dungeons = compile_object(DUNGEONS);
    call_out("initCells", 0, cell_1->iterator());
}

object cell_1() { return cell_1; }
object portal() { return portal; }
object highres() { return highres; }
object english() { return english; }


private string getCellData(DatItem item)
{
    string chunk, match;
    int id;

    chunk = cell_1->getItemData(item)->chunk();
    id = item->id();
    if (id == 0xffff0001) {
	return chunk;
    }

    match = "    ";
    match[0] = id;
    match[1] = id >> 8;
    match[2] = id >> 16;
    match[3] = id >> 24;
    if (chunk[.. 3] != match) {
	error("Item " + id + " data doesn't match");
    }

    return chunk[4 ..];
}

static void initCells(Iterator cells)
{
    DatItem item;
    int id, x, y;

    item = cells->next();
    if (item) {
	id = item->id();
	x = (id >> 24) & 0xff;
	y = (id >> 16) & 0xff;
	switch (id & 0xffff) {
	case 0xfffe:
	    land->setInfo(x, y, getCellData(item), item->flags(),
			  item->timeStamp(), item->iteration());
	    break;

	case 0xffff:
	    land->setBlock(x, y, getCellData(item), item->flags(),
			   item->timeStamp(), item->iteration());
	    break;

	default:
	    dungeons->setCell(x, y, id & 0xffff, getCellData(item),
			      item->flags(), item->timeStamp(),
			      item->iteration());
	    break;
	}

	call_out("initCells", 0, cells);
    }
}
