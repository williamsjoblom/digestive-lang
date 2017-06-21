//
// Created by wax on 2/22/17.
//


#include "IntegerLiteral.h"
#include "gen/Gen.h"


IntegerLiteral::IntegerLiteral(int value) {
    this->value = value;
}

X86Gp IntegerLiteral::generate(X86Compiler &c) {
    return Generate::expression(c, this);
}

bool IntegerLiteral::equals(const Node &other) const {
    const IntegerLiteral* o = dynamic_cast<const IntegerLiteral*>(&other);
    if (o == nullptr) return false;

    return o->value == value;
}

void IntegerLiteral::dump(size_t indent) {
    std::cout << value;
}