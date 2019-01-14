#pragma once

#include "Expr.hh"
#include "Operator.hh"


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
