#pragma once

#include "Stmt.hh"
#include "Expr.hh"
#include "BlockStmt.hh"

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
