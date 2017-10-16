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
 * receive a message from the client
 */
static void receive(string blob)
{
    int type;
    string body;
    Message message;

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
	    send(new CharacterError(4), TRUE);
	    return;
	}
	send(new Message(MSG_DDD_END), TRUE);
	break;

    case MSG_TYPE(MSG_DDD_END):
	/* no response */
	break;

    case MSG_TYPE(MSG_CHARACTER_CREATE):
	message = new ClientCharacterCreate(body);
	send(new CharacterCreateResponse(CHARGEN_RESPONSE_OK, 123,
					 message->name(), 0),
	     TRUE);
	break;

    case MSG_TYPE(MSG_CHARACTER_LOGIN_REQUEST):
	send(new Message(MSG_CHARACTER_SERVER_READY), TRUE);
	break;

    default:
	error("Unknown message type " + type);
    }
}


static void create(Account account, Interface interface)
{
    ::account = account;
    ::interface = interface;
}

static void establish()
{
    /* send server info and char list */
    send(new CharacterList(account), TRUE);
    send(new ServerName("Asheron's Fall", 0, -1), TRUE);
    send(new GenericMessage(MSG_DDD_INTERROGATION,
			    "\1\0\0\0" +
			    "\1\0\0\0" +
			    "\1\0\0\0" +
			    "\2\0\0\0" +
			    "\0\0\0\0" +
			    "\1\0\0\0"),
	 TRUE);
}

void establishConnection()
{
    if (previous_program() == OBJECT_PATH(PacketInterface)) {
	call_out("establish", 0);
    }
}

void receiveMessage(string message, int group)
{
    if (previous_program() == OBJECT_PATH(PacketInterface)) {
	call_out("receive", 0, message);
    }
}

void logout()
{
    if (previous_object() == interface) {
	destruct_object(this_object());
    }
}


Account account()	{ return account; }
Interface interface()	{ return interface; }
