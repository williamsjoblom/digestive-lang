//
// Created by wax on 12/15/16.
//

#ifndef DIG_LITERALEXPR_H
#define DIG_LITERALEXPR_H


#include "Expr.h"

class Literal : public Expr {
public:
    int value;

    Literal(int value);

    virtual X86GpVar generate(X86Compiler &c) = 0;

    virtual bool equals(const Node& other) const;

    void analyze(Scope* scope) { }
    virtual void dump(size_t indent);
};


#endif //DIG_LITERALEXPR_H