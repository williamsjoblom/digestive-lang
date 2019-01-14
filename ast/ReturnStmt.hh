#pragma once

#include "Expr.hh"
#include "Stmt.hh"

class ReturnStmt : public Stmt {
public:
    Expr* expression;
    DType returnType;

    ReturnStmt(Expr* expression);
    ~ReturnStmt();

    void generate(TACFun* fun) override;

    void analyze(Scope* scope) override;
    virtual bool equals(const Node& other) const override;
    void dump(size_t indent) override;
};
