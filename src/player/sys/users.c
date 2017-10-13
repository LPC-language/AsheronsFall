# include "Message.h"
# include "User.h"

/*
 * keep track of existing and logged-in characters
 */

static void create()
{
    compile_object(OBJECT_PATH(Character));

    compile_object(OBJECT_PATH(GenericMessage));
    compile_object(OBJECT_PATH(CharacterCreateResponse));
    compile_object(OBJECT_PATH(ClientCharacterCreate));
    compile_object(OBJECT_PATH(CharacterList));
    compile_object(OBJECT_PATH(CharacterError));
    compile_object(OBJECT_PATH(ServerName));
}
