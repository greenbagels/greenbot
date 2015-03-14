// @file core.cpp
// @brief Main file, which uses the Bot and IRC classes 
//        to create an IRC bot.
//

#include <regex> // std::smatch TODO: make it a generic vector and get rid of this include.
#include <getopt.h>
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

int main(int argc, char *argv[])
{
  // Default params
  std::string server = "irc.rizon.net";
  std::string port = "6667";
  std::string user = "BlueBot";
  std::string password = "PASSWORD";
  std::string channel = "#greenbot";

  int c;
  while((c = getopt(argc, argv, ":S:P:u:p:c")) != EOF)
  {
    switch (c)
    {
      case 'S':
        server = std::string(optarg);
        break;
      case 'P':
        port = std::string(optarg);
        break;
      case 'p':
        password = std::string(optarg);
        break;
      case 'c':
        channel = std::string(optarg);
        break;
      case 'u':
        user = std::string(optarg);
        break;
    }
  }

  // Connect to IRC.
  Chat *chat = new IRCChat(server, port, user, password);
  chat->Join(channel);

  // Create a bot and register some functionality.
  Bot *b = new Bot(chat);
  b->Register("\\.bots(.*)", &botsCall);
  initializeJavascript();
  b->Register("\\.js (.*)", &evalJavascript);

  // Start the bot.
  b->Start();

  return 0;
}

