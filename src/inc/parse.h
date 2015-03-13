#ifndef IRCPARSE_H
#define IRCPARSE_H

#include <string>
#include <deque>

class ParsedMessage {
  public:
    std::string prefix;
    std::string nickname; // User's nickname
    std::string username; // User
    std::string hostname;
    std::string servername;
    std::string command;
    std::string trail;
    std::deque<std::string> argList;
    ParsedMessage(std::string s);
};

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
