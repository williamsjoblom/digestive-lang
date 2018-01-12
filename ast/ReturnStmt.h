//
// Created by wax on 12/15/16.
//

#ifndef DIG_RETURNSTMT_H
#define DIG_RETURNSTMT_H


#include "Expr.h"
#include "Stmt.h"

class ReturnStmt : public Stmt {
public:
    Expr* expression;
    DType returnType;

    ReturnStmt(Expr* expression);
    ~ReturnStmt();

    void generate(X86Compiler& c) override;
    void generate(TACFun* fun) override;

    void analyze(Scope* scope) override;
    virtual bool equals(const Node& other) const override;
    void dump(size_t indent) override;
};


#endif //DIG_RETURNSTMT_H
