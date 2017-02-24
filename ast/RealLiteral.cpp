//
// Created by wax on 2/22/17.
//

#include <gen/Gen.h>
#include "ast/type/Types.h"
#include "RealLiteral.h"


RealLiteral::RealLiteral(double value) {
    this->value = value;
    this->type = &FLOAT32TYPE;
}

X86Var * RealLiteral::generate(X86Compiler &c) {
    // FIXME: Implement codegen
    assert(false); // Not implemented
    return new X86XmmVar(c.newXmmPs());
    //return Generate::expression(c, this);
}

bool RealLiteral::equals(const Node &other) const {
    const RealLiteral* o = dynamic_cast<const RealLiteral*>(&other);
    if (o == nullptr) return false;

    return o->value == value;
}

void RealLiteral::dump(size_t indent) {
    std::cout << value;
}