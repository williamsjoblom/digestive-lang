//
// Created by wax on 12/13/16.
//

#include <assert.h>

#include "TokenQueue.h"
#include "parse/ParseError.h"

TokenQueue::TokenQueue(std::vector<Token>* content) {
    index = 0;
    tokens = content;
}

TokenQueue::~TokenQueue() { }

Token TokenQueue::pop() {
    assert(index < tokens->size());
    return (*tokens)[index++];
}

Token TokenQueue::peek() const {
    assert(index < tokens->size());
    return (*tokens)[index];
}

void TokenQueue::rewind(int distance) {
    index -= distance;
    assert(index >= 0);
}

Token TokenQueue::top() const {
    return (*tokens)[index];
}

void TokenQueue::dump() const {
    std::cout << "{";

    for (auto it = tokens->begin(); it != tokens->end(); it++) {
        if (it != tokens->begin())
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
    if ((*tokens)[index].type == type) {
        index++;
        return true;
    }

    return false;
}

bool TokenQueue::eatIdentifier(const std::string value) {
    Token t = (*tokens)[index];
    if (t.type == IDENTIFIER && t.value == value) {
        index++;
        return true;
    }

    return false;
}

bool TokenQueue::empty() const {
    return index >= tokens->size();
}






