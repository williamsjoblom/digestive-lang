//
// Created by wax on 12/15/16.
//

#include <gen/Gen.h>
#include "Literal.h"
#include "util/PrettyPrint.h"

Literal::Literal(int value) {
    this->value = value;
}

void Literal::dump(size_t indent) {
    std::cout << value;
}

bool Literal::equals(const Node &other) const {
    const Literal* o = dynamic_cast<const Literal*>(&other);
    if (o == nullptr) return false;

    return o->value == value;
}


