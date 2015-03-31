//MODULE:JavascriptEval
//LIBS:-lv8
// @file modules/inc/js.h
// @brief Javascript interpretter module.
//

#ifndef _JS_H
#define _JS_H

#include <mutex>
#include <v8.h>
#include <unistd.h>
#include "chat.h"
#include "bot.h"

class JavascriptEval : public Callback
{
  private:
    Chat *chat;
    Message *response;
    std::string script;
    std::mutex mtx;
    bool kill = false;
    static void killJavascript(v8::Isolate *isolate, std::mutex *mtx, bool *kill)
    {
      sleep(3);
      mtx->lock();
      if (*kill)
      {
        v8::V8::TerminateExecution(isolate);
      }
      mtx->unlock();
    }

  public:
    bool Match(Message *m);
    void Run();
    JavascriptEval(Chat *c);
};

#endif /* _JS_H */

