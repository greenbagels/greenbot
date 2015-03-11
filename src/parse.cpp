#include "parse.h"

#include <iostream>
#include <sstream>

IRCMessage ParseMessage(std::string displayBuffer)
{
	int prefixEnd = -1; // We don't know if the prefix is actually present
	IRCMessage message;   // struct to hold our message
		if (displayBuffer.at(0) == ':') //if the prefix is actually present
	{
		prefixEnd = displayBuffer.find_first_of(' '); //the end is before the first space
		message.msgPrefix.prefix = displayBuffer.substr(1, prefixEnd - 1); //put the prefix in prefix (note, we can further segment them later if we want)
		if (message.msgPrefix.prefix.find('!') != message.msgPrefix.prefix.npos)
		{
			message.msgPrefix.nickname = message.msgPrefix.prefix.substr(0, message.msgPrefix.prefix.find('!'));
		}
	}

	size_t trailBegin = displayBuffer.find(" :");

	if (trailBegin != displayBuffer.npos) //if there's a trailing message
	{
		message.trail = displayBuffer.substr(trailBegin + 2, displayBuffer.npos); //get the trail (char after colon to end of string)
	}
	message.argList = Split(displayBuffer.substr(prefixEnd + 1, (trailBegin - prefixEnd - 1)), ' '); //we want a list of the arguments so we can seperately address them

	if (!message.argList.empty())
	{
		message.command = message.argList.at(0);
		message.argList.pop_front();
	}
	return message;
}


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
