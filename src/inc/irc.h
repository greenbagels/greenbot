#ifndef _IRC_H
#define _IRC_H

#include "chat.h"
#include "socket.h"
#include <deque>

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
  friend class IRCChat;

  private:
    // The message string.
    std::string str;

    // IRC Specific.
    std::string prefix;
    std::string nickname;
    std::string username;
    std::string hostname;
    std::string servername;
    std::string command;
    std::deque<std::string> argList;

  public:
    // Returns the string to be parsed and used by
    // the bot.
    std::string GetString();
    
    // Return the formatted string to be send over
    // the socket.
    std::string GetFormattedString();

    // Returns the user who sent the message.
    IRCUser *GetUser();

    // Generate a new message given an old message.
    IRCMessage *Respond(std::string s);

    // Constructor.
    IRCMessage(std::string s);
    IRCMessage();

    // Destructor
    ~IRCMessage();
};

class IRCChat : public Chat
{
  private:
    // Holds the connection with the server.
    Socket *socket;
    std::string nick;
    std::string user;
    std::string hostname;

  public:
    // Constructor.
    IRCChat(std::string server,
        std::string port,
        std::string user,
        std::string password);

    // Destructor.
    ~IRCChat();

    // Join a channel.
    void Join(std::string channel);

    // Get the next available message.
    IRCMessage *GetMessage();

    // Send a message.
    void SendMessage(Message *m);

};

#endif /* _IRC_H */

