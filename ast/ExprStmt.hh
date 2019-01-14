#pragma once

#include "util/PrettyPrint.hh"
#include "Expr.hh"
#include "Stmt.hh"
#include "ir/TACOp.hh"


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
