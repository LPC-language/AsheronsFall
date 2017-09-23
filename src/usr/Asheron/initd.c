# include <Iterator.h>
# include "RandSeq.h"
# include "BTree.h"
# include "Dat.h"
# include "landblock.h"
# include "dungeon.h"
# include "Interface.h"
# include "User.h"


object cell_1;
object portal;
object highres;
object english;

object land;
object dungeons;

static void create()
{
    /* general utility */
    compile_object(OBJECT_PATH(RandSeq));

    /* DAT image handling */
    compile_object(OBJECT_PATH(BTree));
    compile_object(OBJECT_PATH(DatItem));
    compile_object(OBJECT_PATH(DatImage));
    cell_1 = clone_object(OBJECT_PATH(DatImage),
			  "/usr/Asheron/dat/data/client_cell_1.dat");
    portal = clone_object(OBJECT_PATH(DatImage),
			  "/usr/Asheron/dat/data/client_portal.dat");
    highres = clone_object(OBJECT_PATH(DatImage),
			   "/usr/Asheron/dat/data/client_highres.dat");
    english = clone_object(OBJECT_PATH(DatImage),
			   "/usr/Asheron/dat/data/client_local_English.dat");

    /* land */
    land = compile_object(LANDBLOCKS);
    dungeons = compile_object(DUNGEONS);
    /* call_out("initCells", 0, cell_1->iterator()); */

    /* player login */
    compile_object(TELNET_SERVER);
    compile_object(UDP_SERVER);
    compile_object(UDP_RELAY_SERVER);
    compile_object(ACCOUNT_SERVER);
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
	x = DAT_CELL_X(id);
	y = DAT_CELL_Y(id);
	switch (id & DAT_CELL_MASK) {
	case DAT_CELL_LANDINFO:
	    land->setInfo(x, y, getCellData(item), item->flags(),
			  item->timeStamp(), item->iteration());
	    break;

	case DAT_CELL_LANDBLOCK:
	    land->setBlock(x, y, getCellData(item), item->flags(),
			   item->timeStamp(), item->iteration());
	    break;

	default:
	    if (id == DAT_ITERATION) {
		land->setIteration(cell_1->getItemData(item)->chunk());
	    } else {
		dungeons->setCell(x, y, id & DAT_CELL_MASK, getCellData(item),
				  item->flags(), item->timeStamp(),
				  item->iteration());
	    }
	    break;
	}

	call_out("initCells", 0, cells);
    }
}
