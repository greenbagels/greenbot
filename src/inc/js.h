// @file modules/inc/js.h
// @brief Javascript interpretter module.
//

#ifndef _JS_H
#define _JS_H

#include "chat.h"
#include <regex>

void initializeJavascript();
void evalJavascript(Chat *c, std::smatch sm, Message *m);

#endif /* _JS_H */

