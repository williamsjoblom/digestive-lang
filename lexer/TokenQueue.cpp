//
// Created by wax on 12/13/16.
//

#include <assert.h>
#include "TokenQueue.h"

TokenQueue::TokenQueue(std::vector<Token>* content) {
    index = 0;
    tokens = content;
}

TokenQueue::~TokenQueue() { }

Token TokenQueue::pop() {
    assert(index < tokens->size());
    return (*tokens)[index++];
}

Token TokenQueue::peek() {
    assert(index < tokens->size());
    return (*tokens)[index];
}

void TokenQueue::rewind(int distance) {
    index -= distance;
    assert(index >= 0);
}

void TokenQueue::dump() {
    std::cout << "{";

    for (auto it = tokens->begin(); it != tokens->end(); it++) {
        if (it != tokens->begin())
            std::cout << ", ";
        std::cout << (*it).value;
    }

    std::cout << "}" << std::endl;
}


