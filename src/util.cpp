#include "util.h"

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

  std::string&
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

  std::string
implode(const std::vector<std::string>& elems, char delim)
{
  std::string s;
  return implode(elems, delim, s);
}

