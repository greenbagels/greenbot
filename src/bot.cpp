// @file bot.cpp
// @brief Implementation of the Bot class, which uses a generic
//        Chat object.
//

#include <regex>
#include "bot.h"

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
    Message *m = chat->GetMessage();
    if (m->GetString() == "") continue;

    for (const auto &callbackPair : callbacks)
    {
      std::regex e (callbackPair.first);
      std::smatch sm;
      std::regex_match(m->GetString(), sm, e);
      if (sm.size() > 0)
      {
        (callbackPair.second)(chat, sm, m);
      }
    }
    delete m;
  }

  return;
}

  void
Bot::Register(std::string command,
    void (*fn)(Chat*, std::smatch, Message*))
{
  callbacks[command] = fn;
}

