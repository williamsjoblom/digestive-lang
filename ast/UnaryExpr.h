//
// Created by wax on 2/21/17.
//

#ifndef DIG_UNARYEXPR_H
#define DIG_UNARYEXPR_H


#include "Expr.h"
#include "Operator.h"


class UnaryExpr : public Expr {
public:
    Expr* expr;
    Operator* op;

    UnaryExpr(Operator* op, Expr* expr);
    ~UnaryExpr();

    TACOp generate(TACFun* env) override;

    void analyze(Scope* scope) override;

    bool equals(const Node& other) const override;

    void dump(size_t indent) override;
};


#endif //DIG_UNARYEXPR_H
