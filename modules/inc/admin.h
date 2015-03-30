//MODULE:Admin
// @file modules/inc/admin.h
// @brief Admin actions.
//

#ifndef _ADMIN_H
#define _ADMIN_H

#include <map>
#include "chat.h"
#include "bot.h"

class Admin : public Callback
{
  private:
    Chat *chat;
    Message *response;
    std::string command;
  public:
    bool Match(Message *m);
    void Run();
    Admin(Chat *c);
};

#endif /* _ADMIN_H */

