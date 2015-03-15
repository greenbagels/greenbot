// @file core.cpp
// @brief Main file, which uses the Bot and IRC classes 
//        to create an IRC bot.
//

#include <regex> // std::smatch TODO: make it a generic vector and get rid of this include.
#include <getopt.h>
#include "bot.h"
#include "irc.h"
#include "js.h"

class BotsCall : public Callback
{
  private:
    Chat *chat;
    Message *response;

  public:
    bool Match(Message *m)
    {
      if (m->GetString().substr(0,5) == ".bots")
      {
        response = m->Respond("Reporting in! [C++]");
        return true;
      }
      return false;
    }

    void Run()
    {
      chat->SendMessage(response);
      delete response;
    }

    BotsCall(Chat *c)
    {
      chat = c;
    }
};


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
  b->Register(new BotsCall(chat));
  b->Register(new JavascriptEval(chat));

  // Start the bot.
  b->Start();

  return 0;
}

