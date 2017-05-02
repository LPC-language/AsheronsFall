inherit "~System/lib/user";
inherit "/lib/util/string";


static void login(string name)
{
    ::login("Connection: " + capitalize(name) + " from " + address() + "\n");
    /*
    account = ACCOUNT_SERVER->login(this_object(), name);
    */
}

static void logout()
{
    /*
    if (query_conn()) {
	ACCOUNT_SERVER->logout(this_object());
	::logout();
    }
    */
}
