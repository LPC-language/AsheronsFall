# include <status.h>
# include "Creature.h"
# include "User.h"

inherit Humanoid;


Account account;	/* account this character is in */
int deleteTimer;	/* delete timer handle */

/*
 * initialize character
 */
static void create(Account account, string name)
{
    ::create(0, name, ({ 100, 100, 100, 100, 100, 100 }), ({ }));
    ::account = account;
}

/*
 * pending delete?
 */
int deleteTimer()
{
    if (deleteTimer != 0) {
	mixed *callouts;
	int sz, i;

	callouts = status(this_object(), O_CALLOUTS);
	for (sz = sizeof(callouts), i = 0; i < sz; i++) {
	    if (callouts[i][CO_HANDLE] == deleteTimer) {
		return callouts[i][CO_DELAY] + 1;
	    }
	}
    }
    return 0;
}

/*
 * start a delete with a delay of one hour
 */
int timedDelete()
{
    if (previous_object() == account && deleteTimer == 0) {
	deleteTimer = call_out("delete", 3600);
	return TRUE;
    }
    return FALSE;
}

/*
 * csncel a timed delete
 */
int cancelDelete()
{
    if (previous_object() == account && deleteTimer != 0) {
	remove_call_out(deleteTimer);
	deleteTimer = 0;
	return TRUE;
    }
    return FALSE;
}

/*
 * delete this character irrevocably
 */
static void delete()
{
    destruct_object(this_object());
}


Account account()	{ return account; }
