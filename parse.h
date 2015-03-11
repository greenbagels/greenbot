#ifndef IRCPARSE_H
#define IRCPARSE_H

#include <string>
#include <deque>

/*
IRC format:
[:Server / nick!user@host] COMMAND [args, up to 15] :trailing
*/

struct MSGPrefix
{
	std::string prefix;
	std::string nickname; // User's nickname
	std::string username; // user
	std::string hostname;
	std::string servername;

};

struct IRCMessage
{
	MSGPrefix msgPrefix;
	short  serverCode;
	std::string command, trail;
	std::deque<std::string> argList;
};

IRCMessage ParseMessage(std::string displayBuffer);
std::deque<std::string> Split(std::string pretext, char delim);

template <typename T>
T ToLower(T tgtString)
{
	for (int i = 0; tgtString[i] != '\0'; i++)
	{
		tgtString[i] = tolower(tgtString[i]);
	}
	return tgtString;
}

#endif
