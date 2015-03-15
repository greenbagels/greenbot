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

    // Return the formatted string to be send over
    // the socket.
    virtual std::string GetFormattedString() = 0;

    // Returns the user who sent the message.
    virtual User *GetUser() = 0;

    // Generate a new message given an old message.
    // This keeps the classes abstract and allows
    // the implementation of Message to hold all the
    // the information it needs.
    virtual Message *Respond(std::string s) = 0;

    // Modify the string of a message.  This allows
    // a created response to be updated later.
    virtual void Update(std::string s) = 0;

    // Virtual destructor.
    virtual ~Message(){};
};

class Chat
{
  public:
    // Note: No constructor is specified.  This
    // allows flexibility in getting parameters
    // specific to the chat protocol (e.g. a
    // server port or password).

    // Virtual destructor.
    virtual ~Chat(){}

    // Join services.
    virtual void Join(std::string) = 0;

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

