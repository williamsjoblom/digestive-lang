//
// Created by wax on 1/31/17.
//

#ifndef DIG_LOOPSTMT_H
#define DIG_LOOPSTMT_H


#include "Expr.h"
#include "Stmt.h"

class LoopStmt : public Stmt {
public:
    Expr* condition;
    Stmt* body;

    LoopStmt(Expr* condition, Stmt* body);
    ~LoopStmt();

    void generate(X86Compiler &c) override;
    void generate(TACFun* env) override;

    void analyze(Scope* scope) override;

    virtual bool equals(const Node& other) const override;

    void dump(size_t indent) override;
};


#endif //DIG_LOOPSTMT_H
