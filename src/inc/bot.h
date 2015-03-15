#ifndef _BOT_H
#define _BOT_H

#include <vector>
#include "chat.h" // Abstract type for a generic bot.

// Abstract class
class Callback {
  public:
    // This function should simultaneously store the Message
    // for later use in the Callback::Run function as well as
    // return true on a match and false otherwise.
    // WARNING: Do not store a reference to the Message, copy
    //          everything needed.
    virtual bool Match(Message *m) = 0;

    // Invoked when Match returns true.  This function may be
    // invoked on a separate thread.
    virtual void Run() = 0;
};

class Bot {
  private:
    Chat *chat;
    std::vector<Callback*> callbacks;
  public:
    // This starts the bot.
    void Start();

    // Register a new call back.
    void Register(Callback *callback);

    // Instantiate a bot. Make sure the Chat is already connected.
    Bot(Chat *c);
};

#endif /* _BOT_H */

