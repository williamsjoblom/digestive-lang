//
// Created by wax on 2/22/17.
//

#ifndef DIG_INTEGERLITERAL_H
#define DIG_INTEGERLITERAL_H


#include "Literal.h"

class IntegerLiteral : public Literal {
public:
    int value;

    IntegerLiteral(int value);

    X86Var* generate(X86Compiler &c);
    bool equals(const Node& other) const;
    void dump(size_t indent);
};


#endif //DIG_INTEGERLITERAL_H
