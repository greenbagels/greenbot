// @file core.cpp
// @brief Main file, which uses the Bot and IRC classes 
//        to create an IRC bot.
//

#include <regex> // std::smatch TODO: make it a generic vector and get rid of this include.
#include "bot.h"
#include "irc.h"

#include "js.h"

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
  // Connect to IRC.
  Chat *chat = new IRCChat("irc.rizon.net", "6667", "BlueBot", "PASSWORD");
  chat->Join("#greenbot");

  // Create a bot and register some functionality.
  Bot *b = new Bot(chat);
  b->Register("\\.bots(.*)", &botsCall);
  initializeJavascript();
  b->Register("\\.js (.*)", &evalJavascript);

  // Start the bot.
  b->Start();

  return 0;
}

