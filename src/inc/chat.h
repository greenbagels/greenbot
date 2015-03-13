#ifndef _CHAT_H
#define _CHAT_H

#include <iostream> // std::stuff

class User
{
  public:
    // Return true if the users are the same else
    // return false.
    virtual bool Equals(User *u) = 0;
};

class Message
{
  public:
    // Returns the string to be parsed and used by
    // the bot.
    virtual std::string GetString() = 0;

    // Returns the user who sent the message.
    virtual User *GetUser() = 0;

    // Generate a new message given an old message.
    // This keeps the classes abstract and allows
    // the implementation of Message to hold all the
    // the information it needs.
    virtual Message *Respond(std::string s) = 0;
};

class Chat
{
  public:
    // Specify a connection to make.
    virtual void Connect(std::string server,
                 std::string port,
                 std::string user,
                 std::string password) = 0;

    // Get the next available message.  This should
    // be a blocking function.
    virtual Message *GetMessage() = 0;

    // Send a message.  Note that the Message type
    // should contain all the data necessary.
    // Message m will be generated with the
    // Message::Respond function.
    virtual void SendMessage(Message *m) = 0;
};

#endif /* _CHAT_H */

