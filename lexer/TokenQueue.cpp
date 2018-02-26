//
// Created by wax on 12/13/16.
//

#include <assert.h>

#include "TokenQueue.h"
#include "parse/ParseError.h"

TokenQueue::TokenQueue(std::vector<Token> content) {
    index = 0;
    tokens = content;
}

TokenQueue::~TokenQueue() { }

Token TokenQueue::pop() {
    return at(index++);
}

Token TokenQueue::peek() const {
    return at(index);
}

Token TokenQueue::lookahead(int distance) const {
    return at(index + distance);
}

void TokenQueue::rewind(int distance) {
    index -= distance;
    assert(index >= 0);
}

void TokenQueue::rewindTo(Token t) {
    index = t.index;
}

Token TokenQueue::top() const {
    return at(index);
}

Token TokenQueue::at(int i) const {
    if (i >= tokens.size()) {
        Token t;
        t.type = TokenType::END_OF_FILE;
        t.row = -1;
        t.col = -1;
        t.value = "EOF";
        return t;
    }

    return tokens.at(i);
}

void TokenQueue::dump() const {
    std::cout << "{";

    for (auto it = tokens.begin(); it != tokens.end(); it++) {
        if (it != tokens.begin())
            std::cout << ", ";
        std::cout << (*it).value;
    }

    std::cout << "}" << std::endl;
}

Token TokenQueue::expect(const TokenType type) {
    Token t = top();
    if (t.type != type) unexpectedToken(t);

    index++;
    return t;
}

bool TokenQueue::eat(const TokenType type) {
    if (at(index).type == type) {
        index++;
        return true;
    }

    return false;
}

bool TokenQueue::eatIdentifier(const std::string value) {
    Token t = at(index);
    if (t.type == IDENTIFIER && t.value == value) {
        index++;
        return true;
    }

    return false;
}

bool TokenQueue::empty() const {
    return index >= tokens.size();
}

int TokenQueue::size() const {
    return tokens.size();
}










