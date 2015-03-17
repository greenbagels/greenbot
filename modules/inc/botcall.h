//MODULE:BotsCall
// @file modules/inc/botcall.h
// @brief Responds to .bots.
//

#ifndef _BOTCALL_H
#define _BOTCALL_H

#include "chat.h"
#include "bot.h"

class BotsCall : public Callback
{
  private:
    Chat *chat;
    Message *response;

  public:
    bool Match(Message *m);

    void Run();

    BotsCall(Chat *c)
    {
      chat = c;
    }
};

#endif /* _BOTCALL_H */
