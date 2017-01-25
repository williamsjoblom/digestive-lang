//
// Created by wax on 12/15/16.
//

#ifndef DIG_BINARYEXPR_H
#define DIG_BINARYEXPR_H


#include "Expr.h"
#include "Operator.h"

class BinaryExpr : public Expr {
public:
    Expr* left;
    Operator* op;
    Expr* right;

    virtual X86GpVar* generate(X86Compiler &c);

    void analyze(Scope* scope);

    BinaryExpr(Expr* left, Operator* op, Expr* right);

    void dump(size_t indent);
};


#endif //DIG_BINARYEXPR_H
