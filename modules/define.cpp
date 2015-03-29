// @file modules/define.cpp
// @brief Allows #define macro to be used.
//

#include "define.h"

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

  bool
Define::Match(Message *m)
{
  std::string str = m->GetString();
  std::vector<std::string> splits = split(str, ' ');
  if (splits.size() >= 3 && splits[0] == "#define")
  {
    std::string key = "#" + splits[1];
    std::string match = implode(std::vector<std::string>(splits.begin() + 2, splits.end()), ' ');
    response = m->Respond("Registered " + key + " -> " + match);
    matches[key] = match;
    return true;
  }
  std::map<std::string, std::string>::iterator it = matches.find(str);
  if (it != matches.end())
  {
    response = m->Respond(it->second);
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
