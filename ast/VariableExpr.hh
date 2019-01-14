#pragma once

#include "Expr.hh"

class VariableExpr : public Expr {
public:
    std::string identifier;
    VariableDecl* declaration;

    VariableExpr(std::string identifier);

    virtual TACOp generate(TACFun* env) override;

    void analyze(Scope* scope) override;

    virtual bool equals(const Node& other) const override;

    void dump(size_t indent) override;

};
