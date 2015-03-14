// @file modules/js.cpp
// @brief Javascript interpretter module.
//
#include <v8.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include "js.h"

static std::mutex mtx;
static bool kill = false;

void killJavascript(v8::Isolate *isolate)
{
  sleep(.5);
  mtx.lock();
  if (kill)
  {
    v8::V8::TerminateExecution(isolate);
  }
  mtx.unlock();
}

void initializeJavascript()
{
  v8::V8::Initialize();
}

void evalJavascript(Chat *c, std::smatch sm, Message *m)
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  if(!isolate)
  {
    isolate = v8::Isolate::New();
    isolate->Enter();
  }
  v8::HandleScope handle_scope(isolate);
  v8::Handle<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);

  v8::Handle<v8::String> source = v8::String::NewFromUtf8(isolate, sm.str(1).c_str());
  v8::Handle<v8::Script> script = v8::Script::Compile(source);

  kill = true;
  std::thread killer(killJavascript, isolate);
  v8::Handle<v8::Value> result = script->Run();
  mtx.lock();
  kill = false;
  mtx.unlock();
  killer.join();

  v8::String::Utf8Value utf8(result);
  
  if (*utf8)
  {
    Message *response = m->Respond(std::string(*utf8));
    c->SendMessage(response);
    delete response;
  }
  else
  {
    Message *response = m->Respond(std::string("Error processing request."));
    c->SendMessage(response);
    delete response;
  }
}

