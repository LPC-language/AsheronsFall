# include "Message.h"

inherit Message;


private int chatType;	/* chat type */
private string chat;	/* chat message */

/*
 * export as a blob
 */
static string body()
{
    return serialize("ti", chat, chatType);
}

/*
 * create ServerMessage message
 */
static void create(int chatType, string chat)
{
    ::create(MSG_SERVER_MESSAGE);

    ::chatType = chatType;
    ::chat = chat;
}


int chatType()		{ return chatType; }
string chat()		{ return chat; }
