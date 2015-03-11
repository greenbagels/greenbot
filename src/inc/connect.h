#ifndef CONNECT_H
#define CONNECT_H

#include "parse.h"

int SendToServer(int irc_socket, const char* message);
int CheckPingStatus(int irc_socket, IRCMessage filled_message);
int CreateSocket();


#endif
