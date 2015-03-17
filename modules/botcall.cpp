// @file modules/botcall.cpp
// @brief Responds to .bots.
//

#include "botcall.h"

  bool
BotsCall::Match(Message *m)
{
  if (m->GetString().substr(0,5) == ".bots")
  {
    response = m->Respond("Reporting in! [C++]");
    return true;
  }
  return false;
}

  void
BotsCall::Run()
{
  chat->SendMessage(response);
  delete response;
}

