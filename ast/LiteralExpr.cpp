//
// Created by wax on 12/15/16.
//

#include <gen/Gen.h>
#include "LiteralExpr.h"
#include "util/PrettyPrint.h"

LiteralExpr::LiteralExpr(int value) {
    this->value = value;
}

void LiteralExpr::dump(size_t indent) {
    std::cout << value;
}

X86GpVar* LiteralExpr::generate(X86Compiler &c) {
    return Generate::expression(c, this);
}

bool LiteralExpr::equals(const Node &other) const {
    const LiteralExpr* o = dynamic_cast<const LiteralExpr*>(&other);
    if (o == nullptr) return false;

    return o->value == value;
}


