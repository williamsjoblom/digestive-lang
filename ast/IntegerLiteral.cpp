//
// Created by wax on 2/22/17.
//


#include "IntegerLiteral.h"
#include "gen/Gen.h"


IntegerLiteral::IntegerLiteral(int value) : Literal(value) {
    this->value = value;
}

X86GpVar IntegerLiteral::generate(X86Compiler &c) {
    return Generate::expression(c, this);
}
