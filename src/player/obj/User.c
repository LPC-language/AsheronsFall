# include "Serialized.h"
# include "Interface.h"
# include "Message.h"
# include "GameEvent.h"
# include "properties.h"
# include "chat.h"
# include "statusmessage.h"
# include "User.h"

inherit Serialized;


# define DAT_PLAIN	0x80ed
# define DAT_FULL	0x4f56

Account account;	/* account */
Interface interface;	/* interface */
int plain;		/* using unaltered DAT files from the installer? */
int eventSeq;		/* event sequence number */

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
 * send an event to the player
 */
private void sendEvent(GameEvent event)
{
    event->setSequence(++eventSeq);
    interface->sendMessage(event->transport(), event->group(), TRUE);
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
static void receive(string blob, int group)
{
    int offset, type, response, id;
    Message message;
    Character player;

    ({
	offset,
	type
    }) = deSerialize(blob, 0, "i");

    switch (MSG_GROUPTYPE(group, type)) {
    case MSG_DDD_INTERROGATION_RESPONSE:
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

    case MSG_DDD_END:
	/* no response */
	break;

    case MSG_CHARACTER_CREATE:
	message = new ClientCharacterCreate(blob);
	({ response, id }) = account->characterCreate(message->name());
	if (response == CHARGEN_RESPONSE_OK) {
	    send(new CharacterCreateResponse(response, id, message->name(), 0));
	} else {
	    send(new CharacterCreateResponse(response));
	}
	break;

    case MSG_CHARACTER_DELETE:
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

    case MSG_CHARACTER_RESTORE:
	message = new ClientCharacterRestore(blob);
	({
	    response,
	    player
	}) = account->characterRestore(message->playerId());
	if (response == CHARGEN_RESPONSE_OK) {
	    send(new CharacterCreateResponse(response, message->playerId(),
					     player->name(), 0));
	} else {
	    send(new CharacterCreateResponse(response));
	}
	break;

    case MSG_CHARACTER_LOGIN_REQUEST:
	/*
	 * provides the opportunity to limit the number of simultaneous logins
	 */
	send(new Message(MSG_CHARACTER_SERVER_READY));
	break;

    case MSG_CHARACTER_ENTER_WORLD:
	message = new ClientCharacterEnterWorld(blob);
	if (message->accountName() != account->name()) {
	    send(new CharacterError(CHARERR_NOT_OWNED));
	    break;
	}
	player = account->character(message->playerId());
	if (!player) {
	    send(new CharacterError(CHARERR_NOT_OWNED));
	    break;
	}
	send(new PrivateUpdateBool(player, PROP_BOOL_ACCOUNT_15_DAYS));
	send(new ServerMessage(CHAT_MAGIC, "Thank you for purchasing the Throne of Destiny expansion! A special gift has been placed in your backpack."));
	sendEvent(new PlayerDescription(player));
	sendEvent(new StatusMessage(player, STATMSG_TURBINE_CHAT_ENABLED));
	sendEvent(new CharacterTitle(player, ({ 9 }), ({ 9 })));
	send(new GenericMessage(MSG_PLAYER_CREATE,
				serialize("i", player->id())));
	sendEvent(new PopupString(player, "Asheron has been defeated by Wael'Bharon!\nShadows run rampant in Dereth."));
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
	call_out("receive", 0, message, group);
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
