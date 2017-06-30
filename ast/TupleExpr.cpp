//
// Created by wax on 6/30/17.
//

#include "TupleExpr.h"

TupleExpr::TupleExpr(std::vector<Expr *> values) {
    this->values = values;
}

TupleExpr::~TupleExpr() {
    for (Expr* v : values) delete v;
}

X86Gp TupleExpr::generate(X86Compiler &c) {
    return X86Gp();
}

void TupleExpr::analyze(Scope *scope) {
    for (Expr* v : values) v->analyze(scope);
}

bool TupleExpr::equals(const Node &other) const {
    return Node::equals(other);
}

void TupleExpr::dump(size_t indent) {
    std::cout << "(";
    for (Expr* v : values) {
        v->dump(indent);
        std::cout << " ";
    }
    std::cout << ")";
}
