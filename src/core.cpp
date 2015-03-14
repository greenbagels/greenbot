// @file core.cpp
// @brief Main file, which uses the Bot and IRC classes 
//        to create an IRC bot.
//

#include <regex> // std::smatch TODO: make it a generic vector and get rid of this include.
#include "bot.h"
#include "irc.h"

int IRCRegistration(int irc_socket);
int MainLoop(int irc_socket);

void botsCall(Chat *c, std::smatch sm, Message *m)
{
  Message *response = m->Respond("Reporting in! [C++]");
  c->SendMessage(response);
  delete response;
}

int main()
{
  Chat *ircChat = new IRCChat;
  ircChat->Connect("irc.rizon.net", "6667", "bluebot", "");
  Bot *b = new Bot(ircChat);
  b->Register("\\.bots(.*)", &botsCall);
  b->Start();
  return 0;
}

