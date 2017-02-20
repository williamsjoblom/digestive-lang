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

    ReturnStmt(Expr* expression);
    ~ReturnStmt();

    virtual void generate(X86Compiler &c);

    void analyze(Scope* scope);
    virtual bool equals(const Node& other) const;
    void dump(size_t indent);
};


#endif //DIG_RETURNSTMT_H
