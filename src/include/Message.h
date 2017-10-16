# define Message			object "/usr/Asheron/player/lib/message/Message"
# define GenericMessage			object "/usr/Asheron/player/lib/message/GenericMessage"
# define ClientCharacterDelete		object "/usr/Asheron/player/lib/message/ClientCharacterDelete"
# define ClientCharacterCreate		object "/usr/Asheron/player/lib/message/ClientCharacterCreate"
# define CharacterCreateResponse	object "/usr/Asheron/player/lib/message/CharacterCreateResponse"
# define CharacterList			object "/usr/Asheron/player/lib/message/CharacterList"
# define CharacterError			object "/usr/Asheron/player/lib/message/CharacterError"
# define ClientCharacterRestore		object "/usr/Asheron/player/lib/message/ClientCharacterRestore"
# define ServerName			object "/usr/Asheron/player/lib/message/ServerName"


# define MSG_TYPE(msg)			((msg) & 0xffff)
# define MSG_GROUP(msg)			((msg) >> 16)

# define MSG_CHARACTER_CREATE_RESPONSE		0x9f643
# define MSG_CHARACTER_DELETE			0x4f655
# define MSG_CHARACTER_DELETE_RESPONSE		0x9f655
# define MSG_CHARACTER_CREATE			0x4f656
# define MSG_CHARACTER_LIST			0x9f658
# define MSG_CHARACTER_ERROR			0x9f659
# define MSG_CHARACTER_LOGIN_REQUEST		0x4f7c8
# define MSG_CHARACTER_SERVER_READY		0x9f7df
# define MSG_CHARACTER_RESTORE			0x2f7d9
# define MSG_SERVER_NAME			0x9f7e1
# define MSG_DDD_INTERROGATION			0x5f7e5
# define MSG_DDD_INTERROGATION_RESPONSE		0x5f7e6
# define MSG_DDD_END				0x5f7ea
