// @file parse.cpp
// @brief Implementation of the parse class.
//

#include <sstream> // std::stringstream
#include "parse.h"

std::deque<std::string> Split(std::string pretext, char delim)
{
	std::deque<std::string> sans_delim;
	std::string tokens;
	std::stringstream sstream(pretext);
	while (std::getline(sstream, tokens, delim))
	{
		sans_delim.push_back(tokens);
	}
	return sans_delim;
}

//IRC format:
//[:Server / nick!user@host] COMMAND [args, up to 15] :trailing
ParsedMessage::ParsedMessage(std::string s)
{
  // We don't know if the prefix is actually present.
	int prefixEnd = -1;

  // The prefix is actually present.
  if (s.at(0) == ':')
	{
    // The end is before the first space.
		prefixEnd = s.find_first_of(' ');
    // Save the prefix. (Note: we ca further segment it later.)
		prefix = s.substr(1, prefixEnd - 1);
		if (prefix.find('!') != prefix.npos)
		{
			nickname = prefix.substr(0, prefix.find('!'));
		}
	}

	size_t trailBegin = s.find(" :");

  // Check if there is a trailing message.
	if (trailBegin != s.npos)
	{
    // Save the trail (char after colon to end of string).
		trail = s.substr(trailBegin + 2, s.npos);
	}
  // 
	argList = Split(s.substr(prefixEnd + 1, (trailBegin - prefixEnd - 1)), ' '); //we want a list of the arguments so we can seperately address them

	if (!argList.empty())
	{
		command = argList.at(0);
		argList.pop_front();
	}
}

