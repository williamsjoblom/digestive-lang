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

    virtual void generate(X86Compiler &c);

    void analyze(Scope* scope);

    virtual bool equals(const Node& other) const;

    void dump(size_t indent);
};


#endif //DIG_LOOPSTMT_H
