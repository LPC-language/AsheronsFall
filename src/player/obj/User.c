# include "Serialized.h"
# include "Interface.h"
# include "Message.h"
# include "User.h"

inherit Serialized;


# define DAT_PLAIN	0x80ed
# define DAT_FULL	0x4f56

Account account;	/* account */
Interface interface;	/* interface */
int plain;		/* using unaltered DAT files from the installer? */

/*
 * send a message to the client
 */
private void send(Message message, int required)
{
    interface->sendMessage(message->transport(), message->group(), required);
}

/*
 * show login screen
 */
private void loginScreen()
{
    send(new CharacterList(account->characters(), account->slots(),
			   account->name()),
	 TRUE);
    send(new ServerName("Asheron's Fall", 0, -1), TRUE);
}

/*
 * receive a message from the client
 */
static void receive(string blob)
{
    int type, response, id;
    string body;
    Message message;
    Character character;

    ({
	body,
	type
    }) = deSerialize(blob, "i");

    switch (type) {
    case MSG_TYPE(MSG_DDD_INTERROGATION_RESPONSE):
	switch (hash_crc16(body)) {
	case DAT_PLAIN:
	    plain = TRUE;
	    break;

	case DAT_FULL:
	    break;

	default:
	    send(new CharacterError(CHARERR_SERVER_CRASH), TRUE);
	    return;
	}
	send(new Message(MSG_DDD_END), TRUE);
	break;

    case MSG_TYPE(MSG_DDD_END):
	/* no response */
	break;

    case MSG_TYPE(MSG_CHARACTER_CREATE):
	message = new ClientCharacterCreate(body);
	({ response, id }) = account->characterCreate(message->name());
	if (response == CHARGEN_RESPONSE_OK) {
	    send(new CharacterCreateResponse(response, id, message->name(), 0),
		 TRUE);
	} else {
	    send(new CharacterCreateResponse(response), TRUE);
	}
	break;

    case MSG_TYPE(MSG_CHARACTER_DELETE):
	message = new ClientCharacterDelete(body);
	if (message->accountName() == account->name()) {
	    response = account->characterDelete(message->slot());
	    if (response == CHARERR_OK) {
		send(new Message(MSG_CHARACTER_DELETE_RESPONSE), TRUE);
		loginScreen();
	    } else {
		send(new CharacterError(response), TRUE);
	    }
	} else {
	    send(new CharacterError(CHARERR_DELETE), TRUE);
	}
	break;

    case MSG_TYPE(MSG_CHARACTER_RESTORE):
	message = new ClientCharacterRestore(body);
	({ response, character }) = account->characterRestore(message->id());
	if (response == CHARGEN_RESPONSE_OK) {
	    send(new CharacterCreateResponse(response, message->id(),
					     character->name(), 0),
		 TRUE);
	} else {
	    send(new CharacterCreateResponse(response), TRUE);
	}
	break;

    case MSG_TYPE(MSG_CHARACTER_LOGIN_REQUEST):
	/*
	 * provides the opportinity to limit the number of simultaneous logins
	 */
	send(new Message(MSG_CHARACTER_SERVER_READY), TRUE);
	break;

    default:
	error("Unknown message type " + type);
    }
}


/*
 * initialize user
 */
static void create(Account account, Interface interface)
{
    ::account = account;
    ::interface = interface;
}

/*
 * establish a connection
 */
void establishConnection()
{
    if (previous_object() == interface) {
	loginScreen();
	send(new GenericMessage(MSG_DDD_INTERROGATION,
				"\1\0\0\0" +
				"\1\0\0\0" +
				"\1\0\0\0" +
				"\2\0\0\0" +
				"\0\0\0\0" +
				"\1\0\0\0"),
	     TRUE);
    }
}

/*
 * receive a message via callout
 */
void receiveMessage(string message, int group)
{
    if (previous_program() == OBJECT_PATH(PacketInterface)) {
	call_out("receive", 0, message);
    }
}

/*
 * clean up when connection closes
 */
void logout()
{
    if (previous_object() == interface) {
	destruct_object(this_object());
    }
}


Account account()	{ return account; }
Interface interface()	{ return interface; }
