#ifndef COMMANDS_H
#define COMMANDS_H

#include "parse.h"
#include "connect.h"


int CheckCommands(std::string displayBuffer, int ircSocket, IRCMessage filledMessage);

#endif
