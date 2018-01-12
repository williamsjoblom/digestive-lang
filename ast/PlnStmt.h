#pragma once

#include "Stmt.h"
#include "Expr.h"


/**
 * Print line statement.
 */
class PlnStmt : public Stmt {
public:
    Expr* expression;

    PlnStmt(Expr* expression);
    ~PlnStmt();

    void analyze(Scope* scope) override;

    void generate(X86Compiler& c) override;
    void generate(TACFun* fun) override;

    virtual bool equals(const Node& other) const override;
    void dump(size_t indent) override;
};
