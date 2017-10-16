# define Account		object "/usr/Asheron/player/obj/Account"
# define User			object "/usr/Asheron/player/obj/User"
# define Character		object "/usr/Asheron/player/obj/Character"

# define ACCOUNT_SERVER		"/usr/Asheron/player/sys/accounts"
# define USER_SERVER		"/usr/Asheron/player/sys/users"

/* CharacterErrors */
# define CHARERR_OK			 0
# define CHARERR_SERVER_CRASH		 4
# define CHARERR_DELETE			 6
# define CHARERR_NO_ACCOUNT		10
# define CHARERR_IN_WORLD		13
# define CHARERR_NOT_OWNED		15
# define CHARERR_CORRUPT		18
# define CHARERR_SERVER_FULL		21
# define CHARERR_BOOTED			22
# define CHARERR_SUBSCRIPTION_EXPIRED	24

/* prefab CharacterErrors */
# define CHARERR_LOGGEDON_ALREADY	"\x59\xf6\x00\x00\x01\x00\x00\x00"
# define CHARERR_INVALID_ACCOUNT	"\x59\xf6\x00\x00\x09\x00\x00\x00"

/* character create/restore */
# define CHARGEN_RESPONSE_OK		1
# define CHARGEN_RESPONSE_NAME_IN_USE	3
# define CHARGEN_RESPONSE_NAME_BANNED	4
# define CHARGEN_RESPONSE_CORRUPT	5
# define CHARGEN_RESPONSE_ADMIN_DENIED	7
