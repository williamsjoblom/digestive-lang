#pragma once

#include "Stmt.hh"
#include "Expr.hh"


/**
 * Print line statement.
 */
class PlnStmt : public Stmt {
public:
    Expr* expression;

    PlnStmt(Expr* expression);
    ~PlnStmt();

    void analyze(Scope* scope) override;

    void generate(TACFun* fun) override;

    virtual bool equals(const Node& other) const override;
    void dump(size_t indent) override;
};
