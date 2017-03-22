# include <Iterator.h>
# include "BTree.h"
# include "Dat.h"
# include "landblock.h"


object cell_1;
object portal;
object highres;
object english;

object land;

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
    call_out("initCells", 0, cell_1->iterator());
}

object cell_1() { return cell_1; }
object portal() { return portal; }
object highres() { return highres; }
object english() { return english; }


private string getCellData(DatItem item)
{
    return cell_1->getItemData(item)->chunk();
}

static void initCells(Iterator cells)
{
    DatItem item;
    int id, x, y;

    item = cells->next();
    if (item) {
	id = item->id();
	switch (id & 0xffff) {
	case 0xfffe:
	    x = id >> 24;
	    y = (id >> 16) & 0xff;
	    land->setInfo(x, y, getCellData(item), item->flags(),
			  item->timeStamp(), item->iteration());
	    break;

	case 0xffff:
	    x = id >> 24;
	    y = (id >> 16) & 0xff;
	    land->setBlock(x, y, getCellData(item), item->flags(),
			   item->timeStamp(), item->iteration());
	    break;

	default:
	    break;
	}

	call_out("initCells", 0, cells);
    }
}
