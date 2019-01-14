#pragma once

#include "lexer/Token.hh"

void unexpectedToken(Token& token);
void parseError(Token& token, std::string message);
