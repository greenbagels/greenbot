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
    if (m == NULL)
    {
      continue;
    }
    if (m->GetString() == "")
    {
      delete m;
      std::cout << "Bot exiting..." << std::endl;
      continue;
      break;
    }

    for(auto it = callbacks.begin(); it != callbacks.end(); ++it) {
      if ((*it)->Match(m))
      {
        // TODO place this on an event loop of some kind.
        (*it)->Run();
      }
    }
    delete m;
  }

  return;
}

  void
Bot::Register(Callback *callback)
{
  callbacks.push_back(callback);
}

