//
// Created by wax on 2/22/17.
//

#ifndef DIG_REALLITERAL_H
#define DIG_REALLITERAL_H


#include "Literal.h"

class RealLiteral : public Literal {
public:
    double value;

    RealLiteral(double value);

    X86GpVar generate(X86Compiler &c);
    bool equals(const Node& other) const;
    void dump(size_t indent);
};


#endif //DIG_REALLITERAL_H
