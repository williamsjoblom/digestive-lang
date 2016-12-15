//
// Created by wax on 12/15/16.
//

#ifndef DIG_EXPRSTMT_H
#define DIG_EXPRSTMT_H

#include "Expr.h"
#include "Stmt.h"

class ExprStmt : public Stmt {
public:
    Expr* expression;

    ExprStmt(Expr* expression) { this->expression = expression; }
    void dump(size_t indent) { expression->dump(indent); }
};

#endif //DIG_EXPRSTMT_H
