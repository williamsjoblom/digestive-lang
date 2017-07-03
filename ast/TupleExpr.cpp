//
// Created by wax on 6/30/17.
//

#include "TupleExpr.h"
#include "gen/Gen.h"

TupleExpr::TupleExpr(std::vector<Expr *> values) {
    this->values = values;
}

TupleExpr::~TupleExpr() {
    for (Expr* v : values) delete v;
}

Regs TupleExpr::generate(X86Compiler &c) {
    return Generate::expression(c, this);
}

void TupleExpr::analyze(Scope *scope) {
    std::vector<DType>* types = new std::vector<DType>();
    for (Expr* v : values) {
        v->analyze(scope);
        types->push_back(v->type);
    }

    type = DType(types);
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
