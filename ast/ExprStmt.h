//
// Created by wax on 12/15/16.
//

#ifndef DIG_EXPRSTMT_H
#define DIG_EXPRSTMT_H

#include "util/PrettyPrint.h"
#include "Expr.h"
#include "Stmt.h"
#include "ir/TACOp.h"

class ExprStmt : public Stmt {
public:
    Expr* expression;

    ExprStmt(Expr* expression) { this->expression = expression; }
    ~ExprStmt() { delete expression; }

    void generate(TACFun* env) { expression->generate(env); };

    void analyze(Scope* scope) { expression->analyze(scope); }

    void dump(size_t indent) { printIndent(indent); expression->dump(indent); std::cout << std::endl; }

    virtual bool equals(const Node& other) const {
        const ExprStmt* o = dynamic_cast<const ExprStmt*>(&other);
        if (o == nullptr) return false;

        return o->expression == expression;
    }
};

#endif //DIG_EXPRSTMT_H
