//MODULE:Define
// @file modules/define.cpp
// @brief Allows #define macro to be used.
//

#ifndef _DEFINE_H
#define _DEFINE_H

#include <map>
#include <sstream>
#include <vector>
#include "chat.h"
#include "bot.h"

class Define : public Callback
{
  private:
    std::map<std::string,std::string>matches;
    Chat *chat;
    Message *response;
  public:
    bool Match(Message *m);
    void Run();
    Define(Chat *c);
};

#endif /* _DEFINE_H */

