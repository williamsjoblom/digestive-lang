#pragma once

#include "Expr.hh"
#include "Stmt.hh"

class LoopStmt : public Stmt {
public:
    Expr* condition;
    Stmt* body;

    LoopStmt(Expr* condition, Stmt* body);
    ~LoopStmt();

    void generate(TACFun* fun) override;

    void analyze(Scope* scope) override;

    virtual bool equals(const Node& other) const override;

    void dump(size_t indent) override;
};

