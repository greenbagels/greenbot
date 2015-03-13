// @file bot.cpp
// @brief Implementation of the Bot class, which uses a generic
//        Chat object.
//

#include <regex>
#include "bot.h"
#include "parse.h"

//---------------------------//
// Bot Class Implementation. //
//---------------------------//
Bot::Bot(Chat *c)
{
  chat = c;
}

void
Bot::Start()
{
  while (1) {
    std::cout << "starting bot..." << std::endl;
    Message *m = chat->GetMessage();
    if (m->GetString() == "") break;
    ParsedMessage p = ParsedMessage(m->GetString());
    //std::cout << p.nickname << std::endl;
    //std::cout << p.command << std::endl;
    //for (auto i = p.argList.begin(); i != p.argList.end(); ++i)
    //{
    //  std::cout << *i << std::endl;
    //}
    //std::cout << p.trail << std::endl;
    for (const auto &callbackPair : callbacks)
    {
      std::regex e (callbackPair.first);
      std::smatch sm;
      std::regex_match(p.trail, sm, e);
      if (sm.size() > 0)
      {
        (callbackPair.second)(sm, m);
      }
    }
  }

  return;
}

void
Bot::Register(std::string command,
              void (*fn)(std::smatch, Message*))
{
  callbacks[command] = fn;
}

