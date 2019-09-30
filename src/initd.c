# include "RandSeq.h"
# include "Object.h"
# include "Position.h"
# include "dat.h"
# include "landblock.h"
# include "dungeon.h"
# include "Interface.h"
# include "Creature.h"
# include "User.h"


static void create()
{
    /* general use */
    compile_object(OBJECT_PATH(RandSeq));
    compile_object(OBJECT_PATH(Container));
    compile_object(OBJECT_PATH(RemotePosition));

    /* DAT image server */
    compile_object(DAT_SERVER);

    /* land */
    compile_object(LANDBLOCKS);
    compile_object(DUNGEONS);

    /* creatures */
    compile_object(CREATURE_SERVER);

    /* players */
    compile_object(TELNET_SERVER);
    compile_object(UDP_SERVER);
    compile_object(UDP_RELAY_SERVER);
    compile_object(ACCOUNT_SERVER);
    compile_object(USER_SERVER);
}
