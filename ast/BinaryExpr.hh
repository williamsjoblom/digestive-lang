#pragma once

#include "Expr.hh"
#include "Operator.hh"

class BinaryExpr : public Expr {
public:
    Expr* left;
    Operator* op;
    Expr* right;

    BinaryExpr(Expr* left, Operator* op, Expr* right);
    ~BinaryExpr();

    TACOp generate(TACFun* fun) override;

    void analyze(Scope* scope);

    bool equals(const Node& other) const;

    void dump(size_t indent);
};
