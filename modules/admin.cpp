// @file modules/inc/admin.h
// @brief Admin actions.
//

#include "admin.h"
#include "util.h"

  bool
Admin::Match(Message *m)
{
  if (m->GetString().substr(0,7) == ",admin ")
  {
    if (m->GetPermissions() > 0)
    {
      command = m->GetString();
      return true;
    }
    else
    {
      response = m->Respond("You do not have permission to do that.");
      chat->SendMessage(response);
      return false;
    }
  }
  return false;
}

  void
Admin::Run()
{
  std::vector<std::string> args = split(command, ' ');
  if (args.size() < 2)
  {
    return;
  }
  if (args[1] == "join" && args.size() > 2)
  {
    for (auto it = args.begin() + 2; it != args.end(); it++)
    {
      chat->Join(*it);
    }
  }
  if (args[1] == "part" && args.size() > 2)
  {
    for (auto it = args.begin() + 2; it != args.end(); it++)
    {
      chat->Part(*it);
    }
  }
}

Admin::Admin(Chat *c)
{
  chat = c;
}

