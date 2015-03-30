// @file src/inc/util.h
// @brief Defines some utilities.
//

#ifndef _UTIL_H
#define _UTIL_H

#include <vector>
#include <string>
#include <sstream>

  std::vector<std::string>
&split(const std::string &s, char delim, std::vector<std::string> &elems);
  std::vector<std::string>
split(const std::string &s, char delim);

  std::string&
implode(const std::vector<std::string>& elems, char delim, std::string& s);
  std::string
implode(const std::vector<std::string>& elems, char delim);

#endif /* _UTIL_H */
