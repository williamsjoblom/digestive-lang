#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "TokenQueue.hh"
#include "Token.hh"

class Lexer {
public:
    Lexer(std::string source);
    
    /**
     * Read all tokens.
     */
    TokenQueue readAll();
    
    /**
     * Read token.
     */
    Token read();
    
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
