// @file modules/define.cpp
// @brief Allows #define macro to be used.
//

#include "define.h"
#include <regex>

  std::vector<std::string>
&split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

  std::vector<std::string>
split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}
  static std::string&
implode(const std::vector<std::string>& elems, char delim, std::string& s)
{
  for (std::vector<std::string>::const_iterator ii = elems.begin(); ii != elems.end(); ++ii)
  {
    s += (*ii);
    if ( ii + 1 != elems.end() ) {
      s += delim;
    }
  }

  return s;
}

  static std::string
implode(const std::vector<std::string>& elems, char delim)
{
  std::string s;
  return implode(elems, delim, s);
}

  std::map<std::string, std::string>::iterator
iterate_find(std::map<std::string, std::string> &m, std::string str)
{
  size_t index = str.find_first_of('(');
  std::cout << str.substr(0, index) << std::endl;
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

  bool
Define::Match(Message *m)
{
  std::string str = m->GetString();
  std::vector<std::string> splits = split(str, ' ');
  if (splits.size() >= 3 && splits[0] == "#define")
  {
    std::string key = "#" + splits[1];

    // Prevent bot triggers.
    std::string match = "\u200b" + implode(std::vector<std::string>(splits.begin() + 2, splits.end()), ' ');
    response = m->Respond("Registered " + key + " -> " + match);
    matches[key] = match;
    return true;
  }

  std::string out = "";
  std::map<std::string, std::string>::iterator it = iterate_find(matches, str);
  //std::find_if(matches.begin(), matches.end(), customFind(str));
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
  chat->SendMessage(response);
}

Define::Define(Chat *c)
{
  chat = c;
}
