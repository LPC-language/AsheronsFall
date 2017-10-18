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
 * send a required message to the client
 */
private void send(Message message)
{
    interface->sendMessage(message->transport(), message->group(), TRUE);
}

/*
 * send an optional message to the client
 */
private void sendOptional(Message message)
{
    interface->sendMessage(message->transport(), message->group(), FALSE);
}

/*
 * show login screen
 */
private void loginScreen()
{
    send(new CharacterList(account->characters(), account->slots(),
			   account->name()));
    send(new ServerName("Asheron's Fall", 0, -1));
}

/*
 * receive a message from the client
 */
static void receive(string blob)
{
    int offset, type, response, id;
    Message message;
    Character character;

    ({
	offset,
	type
    }) = deSerialize(blob, 0, "i");

    switch (type) {
    case MSG_TYPE(MSG_DDD_INTERROGATION_RESPONSE):
	switch (hash_crc16(blob[offset ..])) {
	case DAT_PLAIN:
	    plain = TRUE;
	    break;

	case DAT_FULL:
	    break;

	default:
	    send(new CharacterError(CHARERR_SERVER_CRASH));
	    return;
	}
	send(new Message(MSG_DDD_END));
	break;

    case MSG_TYPE(MSG_DDD_END):
	/* no response */
	break;

    case MSG_TYPE(MSG_CHARACTER_CREATE):
	message = new ClientCharacterCreate(blob);
	({ response, id }) = account->characterCreate(message->name());
	if (response == CHARGEN_RESPONSE_OK) {
	    send(new CharacterCreateResponse(response, id, message->name(), 0));
	} else {
	    send(new CharacterCreateResponse(response));
	}
	break;

    case MSG_TYPE(MSG_CHARACTER_DELETE):
	message = new ClientCharacterDelete(blob);
	if (message->accountName() == account->name()) {
	    response = account->characterDelete(message->slot());
	    if (response == CHARERR_OK) {
		send(new Message(MSG_CHARACTER_DELETE_RESPONSE));
		loginScreen();
	    } else {
		send(new CharacterError(response));
	    }
	} else {
	    send(new CharacterError(CHARERR_DELETE));
	}
	break;

    case MSG_TYPE(MSG_CHARACTER_RESTORE):
	message = new ClientCharacterRestore(blob);
	({ response, character }) = account->characterRestore(message->id());
	if (response == CHARGEN_RESPONSE_OK) {
	    send(new CharacterCreateResponse(response, message->id(),
					     character->name(), 0));
	} else {
	    send(new CharacterCreateResponse(response));
	}
	break;

    case MSG_TYPE(MSG_CHARACTER_LOGIN_REQUEST):
	/*
	 * provides the opportunity to limit the number of simultaneous logins
	 */
	send(new Message(MSG_CHARACTER_SERVER_READY));
	break;

    default:
	error("Unhandled message type " + type);
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
				"\1\0\0\0"));
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
