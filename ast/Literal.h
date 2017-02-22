//
// Created by wax on 12/15/16.
//

#ifndef DIG_LITERALEXPR_H
#define DIG_LITERALEXPR_H


#include "Expr.h"

class Literal : public Expr {
public:
    Literal() = default;

    void analyze(Scope* scope) { }
};


#endif //DIG_LITERALEXPR_H
