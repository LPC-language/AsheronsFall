# include "BTree.h"
# include "Dat.h"


object cell_1;
object portal;
object highres;
object english;

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
}

object cell_1() { return cell_1; }
object portal() { return portal; }
object highres() { return highres; }
object english() { return english; }
