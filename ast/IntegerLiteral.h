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

    X86GpVar generate(X86Compiler &c);
};


#endif //DIG_INTEGERLITERAL_H
