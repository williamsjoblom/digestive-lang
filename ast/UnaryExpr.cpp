//
// Created by wax on 2/21/17.
//

#include "UnaryExpr.h"
#include "gen/Gen.h"

UnaryExpr::UnaryExpr(Operator* op, Expr* expr) {
    this->op = op;
    this->expr = expr;
}

UnaryExpr::~UnaryExpr() {
    delete op;
    delete expr;
}

void UnaryExpr::analyze(Scope* scope) {
    expr->analyze(scope);

    /*
     * FIXME: will not work as expected for natural numbers and to be implemented product types.
     */
    type = expr->type;
}

bool UnaryExpr::equals(const Node& other) const {
    const UnaryExpr* o = dynamic_cast<const UnaryExpr*>(&other);
    if (o == nullptr) return false;
    return *o->op == *op && *o->expr == *expr;
}

void UnaryExpr::dump(size_t indent) {
    std::cout << "(";

    op->dump(indent + 1);
    expr->dump(indent + 1);

    std::cout << ")";
}

std::vector<X86Gp> UnaryExpr::generate(X86Compiler& c) {
    return Generate::expression(c, this);
}
