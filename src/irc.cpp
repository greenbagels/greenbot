// @file irc.cpp
// @brief Implementation of the IRC class, which implements the
//        User, Message and Chat abstract classes.
//

#include "irc.h"

//----------------------------//
// User Class Implementation. //
//----------------------------//
IRCUser::IRCUser(void)
{
  std::cout << "Creating User" << std::endl;
}

bool
IRCUser::Equals(User *u)
{
  return true;
}

//-------------------------------//
// Message Class Implementation. //
//-------------------------------//
std::string
IRCMessage::GetString()
{
  return str;
}

IRCMessage::IRCMessage(std::string inputStr)
{
  str = inputStr;
}

IRCUser*
IRCMessage::GetUser()
{
  return new IRCUser;
}

IRCMessage*
IRCMessage::Respond(std::string s)
{
  return new IRCMessage(s);
}

//----------------------------//
// Chat Class Implementation. //
//----------------------------//
IRCChat::IRCChat(void)
{
  std::cout << "Creating Chat" << std::endl;
}

void
IRCChat::Connect(std::string server,
                 std::string port,
                 std::string user,
                 std::string password)
{
  socket = new Socket(server, port);
  socket->Send("PASS PASSWORD\r\n");
  socket->Send("NICK BlueBot\r\n");
	socket->Send("USER BlueBot localhost servername :GreenBot\r\n");
	socket->Send("JOIN #greenbot\r\n");
}

IRCMessage*
IRCChat::GetMessage()
{
  return new IRCMessage(socket->Recv());
}

void
IRCChat::SendMessage(Message *m)
{
  return;
}

