//
// Created by wax on 12/20/16.
//

#ifndef DIG_IFSTATEMENT_H
#define DIG_IFSTATEMENT_H


#include "Stmt.h"
#include "Expr.h"
#include "BlockStmt.h"

class IfStmt : public Stmt {
public:
    Expr* condition;
    Stmt* ifBlock;
    Stmt* elseBlock;

    IfStmt(Expr* condition, Stmt* ifBlock);
    IfStmt(Expr* condition, Stmt* ifBlock, Stmt* elseBlock);
    ~IfStmt();

    void analyze(Scope* scope);

    void generate(X86Compiler &c);
    void generate(TACFun* env);
    
    virtual bool equals(const Node& other) const;

    void dump(size_t indent);

};


#endif //DIG_IFSTATEMENT_H
