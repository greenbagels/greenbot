// @file core.cpp
// @brief Main file, which uses the Bot and IRC classes 
//        to create an IRC bot.
//

#include <regex> // std::smatch TODO: make it a generic vector and get rid of this include.
#include <fstream>
#include "bot.h"
#include "irc.h"
#include "modules.h"
#include "util.h"

int LoadConfig(std::string &server, std::string &port, std::string &user, std::string &password, std::vector<std::string> &channels)
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

    pos = search_result.find("channels = ");
    if (pos != search_result.npos)
    {
      channels = split(search_result.substr(pos + 11), ','); // 11 == strlen("channels = ");
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
  std::vector<std::string> channels;
  if (LoadConfig(server, port, user, password, channels) < 0)
  {
    return -1;
  }

  // Connect to IRC.
  Chat *chat = new IRCChat(server, port, user, password);
  for (auto it = channels.begin(); it != channels.end(); it++)
  {
    chat->Join(*it);
  }

  // Create a bot and register some functionality.
  Bot *b = new Bot(chat);
  LoadModules(b, chat);

  // Start the bot.
  b->Start();

  return 0;
}

