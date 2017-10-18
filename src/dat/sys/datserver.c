# include "BTree.h"
# include "Dat.h"


# define PORTAL_FULL	"\x18\x08\x00\x00\xe8\xf7\xff\xff\x01\x00\x00\x00"
# define PORTAL_PLAIN	"\xda\x05\x00\x00\x26\xfa\xff\xff\x01\x00\x00\x00"

object cell_1;
object portal;
int plain;

static void create()
{
    DatItem item;

    compile_object(OBJECT_PATH(BTree));
    compile_object(OBJECT_PATH(DatItem));
    compile_object(OBJECT_PATH(DatReader));
    compile_object(OBJECT_PATH(DatImage));
    cell_1 = clone_object(OBJECT_PATH(DatImage),
			  "/usr/Asheron/dat/data/client_cell_1.dat");
    portal = clone_object(OBJECT_PATH(DatImage),
			  "/usr/Asheron/dat/data/client_portal.dat");

    item = portal->iterator(DAT_ITERATION)->next();
    switch (portal->getItemData(item)->chunk()) {
    case PORTAL_FULL:
	break;

    case PORTAL_PLAIN:
	plain = TRUE;
	break;

    default:
	error("Unknown portal.dat version");
    }
}

object cell_1() { return cell_1; }
object portal() { return portal; }
int plain()	{ return plain; }


# if 0
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
# endif
