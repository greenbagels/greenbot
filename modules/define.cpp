// @file modules/define.cpp
// @brief Allows #define macro to be used.
//

#include <regex>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include "define.h"
#include "util.h"

  std::map<std::string, std::string>::iterator
iterate_find(std::map<std::string, std::string> &m, std::string str)
{
  size_t index = str.find_first_of('(');
  for (auto it = m.begin(); it != m.end(); ++it)
  {
    size_t i = it->first.find_first_of('(');
    if (index == i && str.substr(0, index) == it->first.substr(0, i))
    {
      return it;
    }
  }
  return m.end();
}

  std::string
randomIntString(void)
{
  return std::to_string(rand() % 100);
}

  std::string
randomAdj(void)
{
  return randomAdjs[rand() % randomAdjs.size()];
}

  std::string
randomNoun(void)
{
  return randomNouns[rand() % randomNouns.size()];
}

static std::map<std::string,std::string(*)(void)> replaceMap = 
{
  {"RANDINT", randomIntString},
  {"RANDADJ", randomAdj},
  {"RANDNOUN", randomNoun}
};

  std::string
Define::ReplaceKeyWords(std::string str)
{
  for(auto it = replaceMap.begin(); it != replaceMap.end(); ++it)
  {
    std::string from = it->first;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        std::string to = it->second();
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
  }
  return str;
}

  bool
Define::Match(Message *m)
{
  std::string str = m->GetString();
  if (str == "" && str[0] != '#')
  {
    return false;
  }

  std::vector<std::string> splits = split(str, ' ');
  if (splits.size() >= 3 && splits[0] == "#define")
  {
    std::string key = "#" + splits[1];

    // Prevent bot triggers.
    std::string match = implode(std::vector<std::string>(splits.begin() + 2, splits.end()), ' ');
    response = m->Respond("Registered " + key + " -> " + match);
    matches[key] = match;
    return true;
  }

  std::string out = "";
  std::map<std::string, std::string>::iterator it = iterate_find(matches, str);

  if (it != matches.end())
  {
    out = it->second;

    // Check if there are arguments.
    std::regex e ("\\(([^\\)]*)");
    std::smatch rm;
    std::regex_search(str, rm, e);
    std::string argString = rm.str(1);
    std::regex_search(it->first, rm, e);
    std::string argReplaceString = rm.str(1);

    // We've got arguments.
    if (argString != "" && argReplaceString != "")
    {
      std::vector<std::string> oldArgs = split(argReplaceString, ',');
      std::vector<std::string> args = split(argString, ',');
      if (args.size() == oldArgs.size())
      {
        for (size_t i = 0; i < args.size(); ++i)
        {
          std::regex rep("\\b" + oldArgs[i] + "\\b");
          out = std::regex_replace(out, rep, "\u200b" + args[i] + "\u200b");
        }
      }
      else
      {
        out = "Invalid arguments for " + it->first + ".";
      }
    }
    
    if (out != "")
    {
      response = m->Respond(out);
    }
    return true;
  }

  return false;
}

 void
Define::Run()
{
  // Check for some key words.
  response->Update(this->ReplaceKeyWords(response->GetString()));
  chat->SendMessage(response);
}

Define::Define(Chat *c)
{
  chat = c;
}
