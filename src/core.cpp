// @file core.cpp
// @brief Main file, which uses the Bot and IRC classes 
//        to create an IRC bot.
//

#include <regex> // std::smatch TODO: make it a generic vector and get rid of this include.
#include <fstream>
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

int LoadConfig(std::string &server, std::string &port, std::string &user, std::string &password, std::string &channel)
{
  std::ifstream config_file;
  config_file.open("greenbot.conf");
  if (!config_file)
  {
    std::cout << "Could not open config file!\n";
    return -1;
  }
  size_t pos;
  std::string search_result;
  while (getline(config_file, search_result))
  {
    pos = search_result.find("server = ");
    if (pos != search_result.npos)
    {
      server = search_result.substr(pos + 9); // 9 == strlen("server = ");
    }

    pos = search_result.find("port = ");
    if (pos != search_result.npos)
    {
      port = search_result.substr(pos + 7); // 7 == strlen("port = ");
    }

    pos = search_result.find("user = ");
    if (pos != search_result.npos)
    {
      user = search_result.substr(pos + 7); // 7 == strlen("user = ");
    }
    
    pos = search_result.find("password = ");
    if (pos != search_result.npos)
    {
      password = search_result.substr(pos + 11); // 11 == strlen("password = ");
    }

    pos = search_result.find("channel = ");
    if (pos != search_result.npos)
    {
      channel = search_result.substr(pos + 10); // 10 == strlen("channel = ");
    }
  }
  config_file.close();
  return 0;
}

int main(int argc, char *argv[])
{
  // Default params
  std::string server;
  std::string port;
  std::string user;
  std::string password;
  std::string channel;
  if (LoadConfig(server, port, user, password, channel) < 0)
  {
    return -1;
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

