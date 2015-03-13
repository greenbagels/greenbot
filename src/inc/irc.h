#ifndef _IRC_H
#define _IRC_H

#include "chat.h"
#include "socket.h"

class IRCUser : public User
{
  public:
    // Return true if the users are the same else
    // return false.
    bool Equals(User *u);

    // Constructor.
    IRCUser();
};

class IRCMessage : public Message
{
  private:
    // The message string.
    std::string str;

  public:
    // Returns the string to be parsed and used by
    // the bot.
    std::string GetString();

    // Returns the user who sent the message.
    IRCUser *GetUser();

    // Generate a new message given an old message.
    IRCMessage *Respond(std::string s);

    // Constructor.
    IRCMessage(std::string);
};

class IRCChat : public Chat
{
  private:
    // Holds the connection with the server.
    Socket *socket;

  public:
    // Specify a connection to make.
    void Connect(std::string server,
                 std::string port,
                 std::string user,
                 std::string password);

    // Get the next available message.
    IRCMessage *GetMessage();

    // Send a message.
    void SendMessage(Message *m);

    // Constructor.
    IRCChat();
};

#endif /* _IRC_H */

