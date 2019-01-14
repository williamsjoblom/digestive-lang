#pragma once

#include <vector>

#include "Token.hh"


class TokenQueue {
public:
    TokenQueue(std::vector<Token>);
    ~TokenQueue();
    Token pop();
    Token peek() const;
    Token lookahead(int distance=1) const;
    void rewind(int distance);
    void rewindTo(Token t);
    Token top() const;
    Token at(int i) const;

    Token expect(const TokenType type);
    bool eat(const TokenType type);
    bool eatIdentifier(const std::string value);
    
    bool empty() const;
    int size() const;

    void dump() const;
private:
    int index;
    std::vector<Token> tokens;
};
