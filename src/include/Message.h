# define Message			object "/usr/Asheron/player/lib/message/Message"
# define GenericMessage			object "/usr/Asheron/player/lib/message/GenericMessage"
# define PrivateRemoveInstance		object "/usr/Asheron/player/lib/message/PrivateRemoveInstance"
# define PrivateUpdateInt		object "/usr/Asheron/player/lib/message/PrivateUpdateInt"
# define UpdateInt			object "/usr/Asheron/player/lib/message/UpdateInt"
# define PrivateUpdateLong		object "/usr/Asheron/player/lib/message/PrivateUpdateLong"
# define PrivateUpdateBool		object "/usr/Asheron/player/lib/message/PrivateUpdateBool"
# define UpdateBool			object "/usr/Asheron/player/lib/message/UpdateBool"
# define PrivateUpdateDouble		object "/usr/Asheron/player/lib/message/PrivateUpdateDouble"
# define UpdateDouble			object "/usr/Asheron/player/lib/message/UpdateDouble"
# define UpdateString			object "/usr/Asheron/player/lib/message/UpdateString"
# define PrivateUpdateData		object "/usr/Asheron/player/lib/message/PrivateUpdateData"
# define UpdateData			object "/usr/Asheron/player/lib/message/UpdateData"
# define PrivateUpdateInstance		object "/usr/Asheron/player/lib/message/PrivateUpdateInstance"
# define UpdateInstance			object "/usr/Asheron/player/lib/message/UpdateInstance"
# define PrivateUpdatePosition		object "/usr/Asheron/player/lib/message/PrivateUpdatePosition"
# define PrivateUpdateSkill		object "/usr/Asheron/player/lib/message/PrivateUpdateSkill"
# define PrivateUpdateAttribute		object "/usr/Asheron/player/lib/message/PrivateUpdateAttribute"
# define PrivateUpdateVital		object "/usr/Asheron/player/lib/message/PrivateUpdateVital"
# define PrivateUpdateVitalLevel	object "/usr/Asheron/player/lib/message/PrivateUpdateVitalLevel"
# define ClientCharacterDelete		object "/usr/Asheron/player/lib/message/ClientCharacterDelete"
# define ClientCharacterCreate		object "/usr/Asheron/player/lib/message/ClientCharacterCreate"
# define CharacterCreateResponse	object "/usr/Asheron/player/lib/message/CharacterCreateResponse"
# define ClientCharacterEnterWorld	object "/usr/Asheron/player/lib/message/ClientCharacterEnterWorld"
# define CharacterList			object "/usr/Asheron/player/lib/message/CharacterList"
# define CharacterError			object "/usr/Asheron/player/lib/message/CharacterError"
# define UpdatePosition			object "/usr/Asheron/player/lib/message/UpdatePosition"
# define ClientCharacterRestore		object "/usr/Asheron/player/lib/message/ClientCharacterRestore"
# define ServerMessage			object "/usr/Asheron/player/lib/message/ServerMessage"
# define ServerName			object "/usr/Asheron/player/lib/message/ServerName"


# define MSG_TYPE(msg)			((msg) & 0xffff)
# define MSG_GROUP(msg)			((msg) >> 16)
# define MSG_GROUPTYPE(group, type)	(((group) << 16) + (type & 0xffff))

# define MSG_36					0x90024
# define MSG_39_SERVER				0x10027
# define MSG_39_CLIENT				0x40027
# define MSG_43					0x4002b
# define MSG_52					0x50034
# define MSG_54					0x50036
# define MSG_55					0x50037
# define MSG_56					0x50038
# define MSG_59					0x5003b
# define MSG_83					0x30053
# define MSG_85					0x10055
# define MSG_86					0x10056
# define MSG_88					0x10058
# define MSG_96					0x10060
# define MSG_102				0x10066
# define MSG_103				0x10067
# define MSG_104				0x10068
# define MSG_105				0x10069
# define MSG_106				0x1006a
# define MSG_107				0x1006b
# define MSG_108				0x1006c
# define MSG_109				0x1006d
# define MSG_110				0x1006e
# define MSG_116				0x10074
# define MSG_119				0x10077
# define MSG_127				0x1007f
# define MSG_128				0x10080
# define MSG_129				0x10081
# define MSG_130				0x10082
# define MSG_131				0x10083
# define MSG_134				0x10086
# define MSG_135				0x10087
# define MSG_142				0x3008e
# define MSG_143				0x3008f
# define MSG_146				0x10092
# define MSG_147				0x10093
# define MSG_150				0x10096
# define MSG_155				0x1009b
# define MSG_159				0x1009f
# define MSG_160				0x100a0
# define MSG_161				0x100a1
# define MSG_168				0x100a8
# define MSG_169				0x100a9
# define MSG_184				0x100b8
# define MSG_186				0x100ba
# define MSG_189				0x100bd
# define MSG_191				0x100bf
# define MSG_193				0x100c1
# define MSG_194				0x100c2
# define MSG_195				0x100c3
# define MSG_196				0x100c4
# define MSG_198				0x100c6
# define MSG_200				0x300c8
# define MSG_201				0x100c9
# define MSG_203				0x100cb
# define MSG_206				0x100ce
# define MSG_209				0x300d1
# define MSG_210				0x100d2
# define MSG_211				0x400d3
# define MSG_212				0x100d4
# define MSG_407				0x90197
# define MSG_414				0x9019e
# define MSG_PRIVATE_REMOVE_INSTANCE		0x901db
# define MSG_480				0x901e0
# define MSG_482				0x901e2
# define MSG_584				0x90248
# define MSG_699				0x902bb
# define MSG_700				0x902bc
# define MSG_PRIVATE_UPDATE_INT			0x902cd
# define MSG_UPDATE_INT				0x902ce
# define MSG_PRIVATE_UPDATE_LONG		0x902cf
# define MSG_PRIVATE_UPDATE_BOOL		0x902d1
# define MSG_UPDATE_BOOL			0x902d2
# define MSG_PRIVATE_UPDATE_DOUBLE		0x902d3
# define MSG_UPDATE_DOUBLE			0x902d4
# define MSG_UPDATE_STRING			0x902d6
# define MSG_PRIVATE_UPDATE_DATA		0x902d7
# define MSG_UPDATE_DATA			0x902d8
# define MSG_PRIVATE_UPDATE_INSTANCE		0x902d9
# define MSG_UPDATE_INSTANCE			0x902da
# define MSG_PRIVATE_UPDATE_POSITION		0x902db
# define MSG_PRIVATE_UPDATE_SKILL		0x902dd
# define MSG_737				0x902e1
# define MSG_738				0x902e2
# define MSG_PRIVATE_UPDATE_ATTRIBUTE		0x902e3
# define MSG_PRIVATE_UPDATE_VITAL		0x902e7
# define MSG_PRIVATE_UPDATE_VITAL_LEVEL		0x902e9
# define MSG_44173				0x9ac8d
# define MSG_50547				0x9c573
# define MSG_61844				0x9f194
# define MSG_63001				0xaf619
# define MSG_63013				0xaf625
# define MSG_CHARACTER_CREATE_RESPONSE		0x9f643
# define MSG_63059_CLIENT			0x4f653
# define MSG_63059_SERVER			0x9f653
# define MSG_CHARACTER_DELETE			0x4f655
# define MSG_CHARACTER_DELETE_RESPONSE		0x9f655
# define MSG_CHARACTER_CREATE			0x4f656
# define MSG_CHARACTER_ENTER_WORLD		0x4f657
# define MSG_CHARACTER_LIST			0x9f658
# define MSG_CHARACTER_ERROR			0x9f659
# define MSG_63210				0x2f6ea
# define MSG_OBJECT_CREATE			0xaf745
# define MSG_PLAYER_CREATE			0xaf746
# define MSG_63303				0xaf747
# define MSG_UPDATE_POSITION			0xaf748
# define MSG_63305				0xaf749
# define MSG_63306				0xaf74a
# define MSG_SET_STATE				0xaf74b
# define MSG_63308				0xaf74c
# define MSG_63310				0xaf74e
# define MSG_63312				0xaf750
# define MSG_63313				0xaf751
# define MSG_63317				0xaf755
# define MSG_GAME_EVENT				0x9f7b0
# define MSG_GAME_ACTION			0x3f7b1
# define MSG_63426				0x2f7c2
# define MSG_CHARACTER_LOGIN_REQUEST		0x4f7c8
# define MSG_63437				0x2f7cd
# define MSG_CHARACTER_RESTORE			0x2f7d9
# define MSG_63451				0xaf7db
# define MSG_63452				0x9f7dc
# define MSG_63454				0x4f7de
# define MSG_CHARACTER_SERVER_READY		0x9f7df
# define MSG_SERVER_MESSAGE			0x9f7e0
# define MSG_SERVER_NAME			0x9f7e1
# define MSG_63458				0x5f7e2
# define MSG_63459				0x5f7e3
# define MSG_63460				0x5f7e4
# define MSG_DDD_INTERROGATION			0x5f7e5
# define MSG_DDD_INTERROGATION_RESPONSE		0x5f7e6
# define MSG_63463				0x5f7e7
# define MSG_DDD_END				0x5f7ea
