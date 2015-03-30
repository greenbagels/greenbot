// @file irc.cpp
// @brief Implementation of the IRC class, which implements the
//        User, Message and Chat abstract classes.
//

#include <sstream> // std::stringstream
#include <algorithm>
#include "irc.h"

std::deque<std::string> Split(std::string pretext, char delim)
{
  std::deque<std::string> sans_delim;
  std::string tokens;
  std::stringstream sstream(pretext);
  while (std::getline(sstream, tokens, delim))
  {
    sans_delim.push_back(tokens);
  }
  return sans_delim;
}

//----------------------------//
// User Class Implementation. //
//----------------------------//
IRCUser::IRCUser(void)
{
  return;
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

  std::string
IRCMessage::GetFormattedString()
{
  std::string arg;
  for (auto i = argList.begin(); i != argList.end(); ++i)
  {
    arg += *i + " ";
  }
  return command + " " + arg + ":" + str + "\r\n";
}

IRCMessage::IRCMessage(std::string s)
{
  str = "";

  // We don't know if the prefix is actually present.
  int prefixEnd = -1;

  // The prefix is actually present.
  if (s.at(0) == ':')
  {
    // The end is before the first space.
    prefixEnd = s.find_first_of(' ');
    // Save the prefix. (Note: we ca further segment it later.)
    prefix = s.substr(1, prefixEnd - 1);
    if (prefix.find('!') != prefix.npos)
    {
      nickname = prefix.substr(0, prefix.find('!'));
    }
  }

  size_t trailBegin = s.find(" :");

  // Check if there is a trailing message.
  if (trailBegin != s.npos)
  {
    // Save the trail (char after colon to end of string).
    str = s.substr(trailBegin + 2, s.npos);
  }
  // We get a list of the arguments.
  argList = Split(s.substr(prefixEnd + 1, (trailBegin - prefixEnd - 1)), ' ');

  if (!argList.empty())
  {
    command = argList.at(0);
    argList.pop_front();
  }
  else
  {
    command = "";
  }

  // Trim the \r\n
  if (str.length() > 2) {
    str = str.substr(0, str.length() - 2);
  }
}

IRCMessage::IRCMessage()
{
  str = "";
  return;
}

IRCMessage::~IRCMessage()
{
  return;
}

  IRCUser*
IRCMessage::GetUser()
{
  return new IRCUser;
}

  IRCMessage*
IRCMessage::Respond(std::string s)
{
  IRCMessage *m = new IRCMessage();
  m->str = s;
  m->command = "PRIVMSG";
  m->argList = argList;
  if (m->argList[0][0] != '#' && m->argList[0][0] != '&')
  {
    m->argList[0] = nickname;
  }

  return m;
}

  void
IRCMessage::Update(std::string s)
{
  str = s;
}

//----------------------------//
// Chat Class Implementation. //
//----------------------------//

IRCChat::IRCChat(std::string server,
    std::string port,
    std::string nick,
    std::string password)
{
  socket = new Socket(server, port);
  socket->Send("PASS " + password + "\r\n");
  socket->Send("NICK " + nick + "\r\n");
  socket->Send("USER " + nick + " localhost servername :" + nick + "\r\n");
}

IRCChat::~IRCChat(void)
{
  delete socket;
}

  void
IRCChat::Join(std::string channel)
{
  socket->Send("JOIN " + channel + "\r\n");
}

  IRCMessage*
IRCChat::GetMessage()
{
  IRCMessage *m = NULL;
  while (!m)
  {
    m = new IRCMessage(socket->Recv());
    if (m->command == "PING")
    {
      socket->Send("PONG " + m->servername + " " + m->str + "\r\n");
      delete m;
      m = NULL;
      continue;
    }
    if (m->command == "KICK")
    {
      this->Join(m->argList[0]);
      delete m;
      m = NULL;
      continue;
    }
  }

#ifdef LOGGING
  std::cout << "COMMAND: " + m->command + "\nARGUMENTS" << std::endl;
  for (auto i = m->argList.begin(); i != m->argList.end(); ++i)
  {
    std::cout << "\t" + *i << std::endl;
  }
  std::cout << "MESSAGE: " + m->str << std::endl;
  std::cout << "NICKNAME: " + m->nickname << std::endl;
  std::cout << "USERNAME: " + m->username << std::endl;
  std::cout << "HOSTNAME: " + m->hostname << std::endl;
  std::cout << "SERVERNAME: " + m->servername << std::endl;
  std::cout << "PREFIX: " + m->prefix << std::endl;
#endif

  return m;
}

  void
IRCChat::SendMessage(Message *m)
{
#ifdef LOGGING
  std::cout << m->GetFormattedString() << std::endl;
#endif
  socket->Send(m->GetFormattedString());
}

