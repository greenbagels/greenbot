// @file modules/js.cpp
// @brief Javascript interpretter module.
//

#include <thread>
#include "js.h"

  bool 
JavascriptEval::Match(Message *m)
{
  if (m->GetString().substr(0,4) == ".js ")
  {
    response = m->Respond("Error processing request.");
    script = m->GetString().substr(4);
    return true;
  }
  return false;
}

  void
JavascriptEval::Run()
{
  // First attempt to run the saved script.
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  if(!isolate)
  {
    isolate = v8::Isolate::New();
    isolate->Enter();
  }
  v8::HandleScope handle_scope(isolate);
  v8::Handle<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);

  v8::Handle<v8::String> source = v8::String::NewFromUtf8(isolate, script.c_str());
  v8::Handle<v8::Script> compiled = v8::Script::Compile(source);

  // Spawn a thread to kill the script if it takes too long.
  kill = true;
  std::thread killer(JavascriptEval::killJavascript, isolate, &mtx, &kill);
  v8::Handle<v8::Value> result = compiled->Run();
  mtx.lock();
  kill = false;
  mtx.unlock();
  killer.join();

  v8::String::Utf8Value utf8(result);
  
  if (*utf8)
  {
    // Update the old response.
    response->Update(std::string(*utf8));
    chat->SendMessage(response);
  }
  else
  {
    // Use the default error response.
    chat->SendMessage(response);
  }

  // Delete the response object in both cases.
  delete response;
}

JavascriptEval::JavascriptEval(Chat *c)
{
  v8::V8::Initialize();
  chat = c;
}

