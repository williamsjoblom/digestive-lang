//
// Created by wax on 12/15/16.
//

#ifndef DIG_EXPRSTMT_H
#define DIG_EXPRSTMT_H

#include <util/PrettyPrint.h>
#include "Expr.h"
#include "Stmt.h"

class ExprStmt : public Stmt {
public:
    Expr* expression;

    ExprStmt(Expr* expression) { this->expression = expression; }
    virtual void generate(X86Compiler &c) { expression->generate(c); }
    void analyze(Scope* scope) { expression->analyze(scope); }
    void dump(size_t indent) { printIndent(indent); expression->dump(indent); std::cout << std::endl; }
};

#endif //DIG_EXPRSTMT_H
