//
// Created by wax on 2/21/17.
//

#ifndef DIG_UNARYEXPR_H
#define DIG_UNARYEXPR_H


#include "Expr.h"
#include "Operator.h"

class UnaryExpr : public Expr {
public:
    Expr* expr;
    Operator* op;

    UnaryExpr(Operator* op, Expr* expr);
    ~UnaryExpr();

    std::vector<X86Gp> generate(X86Compiler &c);

    void analyze(Scope* scope);

    bool equals(const Node& other) const;

    void dump(size_t indent);
};


#endif //DIG_UNARYEXPR_H
