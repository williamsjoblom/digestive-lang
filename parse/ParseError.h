//
// Created by wax on 12/14/16.
//

#ifndef DIG_PARSEERROR_H
#define DIG_PARSEERROR_H

#include <lexer/Token.h>

void unexpectedToken(Token& token);
void parseError(Token& token, std::string message);

#endif //DIG_PARSEERROR_H
