//
// Created by wax on 12/14/16.
//

#include "ParseError.h"

void unexpectedToken(Token& token) {
    std::cout << token.row << ":" << token.col << ": ";
    std::cout << "Unexpected token '" << token.value << "'" << std::endl;
    throw 0;
}

void parseError(Token& token, std::string message) {
    std::cout << token.row << ":" << token.col << ": ";
    std::cout << message << std::endl;
    throw 0;
}
