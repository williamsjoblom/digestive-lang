//
// Created by wax on 12/13/16.
//

#ifndef DIG_TOKENQUEUE_H
#define DIG_TOKENQUEUE_H


#include <vector>
#include "Token.h"


class TokenQueue {
public:
    TokenQueue(std::vector<Token>*);
    ~TokenQueue();
    Token pop();
    Token peek();
    void rewind(int distance);

    void dump();
private:
    int index;
    std::vector<Token>* tokens;
};


#endif //DIG_TOKENQUEUE_H
