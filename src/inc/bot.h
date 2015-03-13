#ifndef _BOT_H
#define _BOT_H

#include <map>
#include <regex>
#include "chat.h" // Abstract type for a generic bot.

class Bot {
  private:
    Chat *chat;
    std::map<std::string,void(*)(Chat*, std::smatch, Message*)> callbacks;
  public:
    // This starts the bot.
    void Start();

    // Register a new call back.
    void Register(std::string command, void (*fn)(Chat*, std::smatch, Message*));

    // Instantiate a bot. Make sure the Chat is already connected.
    Bot(Chat *c);
};

#endif /* _BOT_H */

