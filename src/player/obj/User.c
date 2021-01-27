# include "Serialized.h"
# include "Interface.h"
# include "Message.h"
# include "GameEvent.h"
# include "GameAction.h"
# include "Creature.h"
# include "Position.h"
# include "properties.h"
# include "chat.h"
# include "motion.h"
# include "User.h"

inherit Serialized;


# define DAT_PLAIN	0x80ed
# define DAT_FULL	0xb6fd

Account account;	/* account */
Interface interface;	/* interface */
int plain;		/* using unaltered DAT files from the installer? */
int eventSeq;		/* event sequence number */
Character player;

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
private void sendEvent(int eventType, string body)
{
    Message message;

    message = new GenericMessage(MSG_GAME_EVENT,
				 serialize("iii", player->id(), ++eventSeq,
					   eventType) +
				 body);
    interface->sendMessage(message->transport(), message->group(), TRUE);
}

private void eventPopupString(string text)
{
    sendEvent(EVT_POPUP_STRING, serialize("t", text));
}


private void receiveAction(string blob)
{
    int offset, seq, type;
    GameAction action;

    ({ offset, seq, type }) = deSerialize(blob, 4, "ii");
    switch (type) {
    case ACT_LOGIN_COMPLETE:
	send(new GenericMessage(MSG_SET_STATE,
				serialize("iiss", player->id(), 0x00400408,
					  1, 1)));
	break;

    case ACT_MOVE_TO_STATE:
	action = new MoveToState(blob, offset);
	send(new GenericMessage(MSG_MOVEMENT_EVENT,
				serialize("issssccsi", player->id(), 1, 1, 0,
					  TRUE, 0, action->flags(),
					  MOTION_NONCOMBAT, 0)));
	break;

    case ACT_AUTONOMOUS_POSITION:
	action = new AutonomousPosition(blob, offset);
	send(new UpdatePosition(player->id(), action->position(),
				action->grounded(), action->instanceSeq(), 1,
				action->teleportSeq(), action->forcedSeq()));
	break;

    default:
	error("Unhandled GameAction type " + type);
    }
}


# define FLAG_INT		0x01
# define FLAG_BOOL		0x02
# define FLAG_DOUBLE		0x04
# define FLAG_DATA		0x08
# define FLAG_STRING		0x10
# define FLAG_POSITION		0x20
# define FLAG_INSTANCE		0x40
# define FLAG_LONG		0x80

# define FLAG_ATTRIBUTE		0x001
# define FLAG_SKILL		0x002
# define FLAG_SPELL		0x100
# define FLAG_ENCHANTMENT	0x200

private string properties()
{
    int *intList, *longList, *boolList, *doubleList, *stringList, *dataList,
	flags;

    intList = ({
	PROP_INT_GEAR_PK_DAMAGE_RESIST_RATING,
	PROP_INT_OVERPOWER,
	PROP_INT_OVERPOWER_RESIST,
	PROP_INT_HEALING_BOOST_RATING,
	PROP_INT_GEAR_OVERPOWER,
	PROP_INT_GEAR_OVERPOWER_RESIST,
	PROP_INT_ENCUMB_VAL,
	PROP_INT_ENLIGHTENMENT,
	PROP_INT_PLAYER_KILLER_STATUS,
	PROP_INT_CONTAINERS_CAPACITY,
	PROP_INT_WEAKNESS_RATING,
	PROP_INT_NETHER_OVER_TIME,
	PROP_INT_NETHER_RESIST_RATING,
	PROP_INT_LUM_AUG_DAMAGE_RATING,
	PROP_INT_LUM_AUG_DAMAGE_REDUCTION_RATING,
	PROP_INT_LUM_AUG_CRIT_DAMAGE_RATING,
	PROP_INT_LUM_AUG_CRIT_REDUCTION_RATING,
	PROP_INT_LUM_AUG_SURGE_CHANCE_RATING,
	PROP_INT_LUM_AUG_ITEM_MANA_USAGE,
	PROP_INT_LUM_AUG_ITEM_MANA_GAIN,
	PROP_INT_COIN_VALUE,
	PROP_INT_LUM_AUG_HEALING_RATING,
	PROP_INT_LUM_AUG_SKILLED_CRAFT,
	PROP_INT_LUM_AUG_SKILLED_SPEC,
	PROP_INT_LEVEL,
	PROP_INT_DOT_RESIST_RATING,
	PROP_INT_ALLEGIANCE_RANK,
	PROP_INT_LIFE_RESIST_RATING,
	PROP_INT_MELEE_MASTERY,
	PROP_INT_CREATION_TIMESTAMP,
	PROP_INT_RANGED_MASTERY,
	PROP_INT_WEAPON_AURA_DAMAGE,
	PROP_INT_WEAPON_AURA_SPEED,
	PROP_INT_LUM_AUG_ALL_SKILLS,
	PROP_INT_GENDER,
	PROP_INT_GEAR_DAMAGE,
	PROP_INT_GEAR_DAMAGE_RESIST,
	PROP_INT_DAMAGE_RATING,
	PROP_INT_GEAR_CRIT,
	PROP_INT_DAMAGE_RESIST_RATING,
	PROP_INT_GEAR_CRIT_RESIST,
	PROP_INT_GEAR_CRIT_DAMAGE,
	PROP_INT_GEAR_CRIT_DAMAGE_RESIST,
	PROP_INT_GEAR_HEALING_BOOST,
	PROP_INT_HEAL_OVER_TIME,
	PROP_INT_GEAR_NETHER_RESIST,
	PROP_INT_CRIT_RATING,
	PROP_INT_GEAR_LIFE_RESIST,
	PROP_INT_CRIT_DAMAGE_RATING,
	PROP_INT_GEAR_MAX_HEALTH,
	PROP_INT_CRIT_RESIST_RATING,
	PROP_INT_CRIT_DAMAGE_RESIST_RATING,
	PROP_INT_HERITAGE_GROUP,
	PROP_INT_PK_DAMAGE_RATING,
	PROP_INT_HEALING_RESIST_RATING,
	PROP_INT_PK_DAMAGE_RESIST_RATING,
	PROP_INT_DAMAGE_OVER_TIME,
	PROP_INT_GEAR_PK_DAMAGE_RATING
    });
    longList = ({
	PROP_LONG_AVAILABLE_LUMINANCE,
	PROP_LONG_MAXIMUM_LUMINANCE
    });
    boolList = ({
	PROP_BOOL_IS_PSR,
	PROP_BOOL_ACTD_RECEIVED_ITEMS,
	PROP_BOOL_IS_ADMIN,
	PROP_BOOL_IS_ARCH,
	PROP_BOOL_IS_SENTINEL,
	PROP_BOOL_IS_ADVOCATE
    });
    doubleList = ({
	PROP_DOUBLE_GLOBAL_XP_MOD,
	PROP_DOUBLE_WEAPON_AURA_OFFENSE,
	PROP_DOUBLE_WEAPON_AURA_DEFENSE,
	PROP_DOUBLE_WEAPON_AURA_ELEMENTAL,
	PROP_DOUBLE_WEAPON_AURA_MANA_CONV,
	PROP_DOUBLE_RESIST_HEALTH_DRAIN
    });
    stringList = ({ PROP_STRING_NAME });
    dataList = ({
	PROP_DATA_MOTION_TABLE,
	PROP_DATA_COMBAT_TABLE
    });

    flags = FLAG_INT | FLAG_LONG | FLAG_BOOL | FLAG_DOUBLE | FLAG_STRING |
	    FLAG_DATA;
    return serialize("ii", flags, 0x0a) +
	   serializeMapping(intList, 0x40, player, "rawIntProperty") +
	   serializeMapping(longList, 0x40, player, "rawLongProperty") +
	   serializeMapping(boolList, 0x20, player, "rawBoolProperty") +
	   serializeMapping(doubleList, 0x20, player, "rawDoubleProperty") +
	   serializeMapping(stringList, 0x10, player, "rawStringProperty") +
	   serializeMapping(dataList, 0x20, player, "rawDataProperty");
}

private string abilities()
{
    int *skillList, flags;

    skillList = ({
	SKILL_ITEM_ENCHANTMENT,
	SKILL_LIFE_MAGIC,
	SKILL_WAR_MAGIC,
	SKILL_LEADERSHIP,
	SKILL_LOYALTY,
	SKILL_FLETCHING,
	SKILL_ALCHEMY,
	SKILL_MELEE_DEFENSE,
	SKILL_COOKING,
	SKILL_MISSILE_DEFENSE,
	SKILL_SALVAGING,
	SKILL_TWO_HANDED_COMBAT,
	SKILL_VOID_MAGIC,
	SKILL_HEAVY_WEAPONS,
	SKILL_LIGHT_WEAPONS,
	SKILL_FINESSE_WEAPONS,
	SKILL_ARCANE_LORE,
	SKILL_MISSILE_WEAPONS,
	SKILL_MAGIC_DEFENSE,
	SKILL_SHIELD,
	SKILL_MANA_CONVERSION,
	SKILL_DUAL_WIELD,
	SKILL_RECKLESSNESS,
	SKILL_ITEM_TINKERING,
	SKILL_SNEAK_ATTACK,
	SKILL_ASSESS_PERSON,
	SKILL_DIRTY_FIGHTING,
	SKILL_DECEPTION,
	SKILL_HEALING,
	SKILL_SUMMONING,
	SKILL_JUMP,
	SKILL_LOCKPICK,
	SKILL_RUN,
	SKILL_ASSESS_CREATURE,
	SKILL_WEAPON_TINKERING,
	SKILL_ARMOR_TINKERING,
	SKILL_MAGIC_ITEM_TINKERING,
	SKILL_CREATURE_ENCHANTMENT
    });

    flags = FLAG_ATTRIBUTE | FLAG_SKILL;
    return serialize("iii", flags, 0x01, 0x1ff) +
	   player->rawAttribute(ATTR_STRENGTH) +
	   player->rawAttribute(ATTR_ENDURANCE) +
	   player->rawAttribute(ATTR_QUICKNESS) +
	   player->rawAttribute(ATTR_COORDINATION) +
	   player->rawAttribute(ATTR_FOCUS) +
	   player->rawAttribute(ATTR_SELF) +
	   player->rawVitalAttribute(VITAL_HEALTH) +
	   player->rawVitalAttribute(VITAL_STAMINA) +
	   player->rawVitalAttribute(VITAL_MANA) +
	   serializeMapping(skillList, 0x20, player, "rawSkill");
}

private void eventPlayerDescription()
{
    sendEvent(EVT_PLAYER_DESCRIPTION,
	      properties() + abilities() + player->options() +
	      "\0\0\0\0" +	/* inventory */
	      "\0\0\0\0");	/* equipped */
}

private void eventCharacterTitle()
{
    sendEvent(EVT_CHARACTER_TITLE,
	      serializeArray(player->displayTitles()) +
	      serializeArray(player->titleList()));
}

# define STATMSG_TURBINE_CHAT_ENABLED	0x051d

private void eventStatusMessage(int status)
{
    sendEvent(EVT_STATUS_MESSAGE, serialize("i", status));
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
	message = new RemoteCharacterCreate(blob);
	({ response, id }) = account->characterCreate(message->name());
	if (response == CHARGEN_RESPONSE_OK) {
	    send(new CharacterCreateResponse(response, id, message->name(), 0));
	} else {
	    send(new CharacterCreateResponse(response));
	}
	break;

    case MSG_CHARACTER_DELETE:
	message = new RemoteCharacterDelete(blob);
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
	message = new RemoteCharacterRestore(blob);
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
	message = new RemoteCharacterEnterWorld(blob);
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
	eventPlayerDescription();
	eventStatusMessage(STATMSG_TURBINE_CHAT_ENABLED);
	eventCharacterTitle();
	send(new GenericMessage(MSG_PLAYER_CREATE,
				serialize("i", player->id())));
	eventPopupString("Asheron has been defeated by Wael'Bharon!\nShadows run rampant in Dereth.");
	send(new GenericMessage(MSG_OBJECT_CREATE, player->transport()));
	break;

    case MSG_CHARACTER_LOGOFF:
	if (deSerialize(blob, 4, "i")[1] == player->id()) {
	    send(new Message(MSG_CHARACTER_LOGOFF_RESPONSE));
	    loginScreen();
	}
	break;

    case MSG_GAME_ACTION:
	receiveAction(blob);
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
