//
// Created by wax on 12/13/16.
//

#ifndef DIG_LEXER_H
#define DIG_LEXER_H

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "TokenQueue.h"
#include "Token.h"

class Lexer {
public:
    Lexer();
    TokenQueue lex(std::string source);
private:
    std::string source;
    unsigned int index;
    int row, col;

    void readWhitespace();
    void readComment();
    void readAlpha(Token& token);
    void readNum(Token& token);
    void readString(Token& token);
    void readSymbol(Token& token);
};


#endif //DIG_LEXER_H
