//
// Created by wax on 12/14/16.
//

#include <gen/Gen.h>
#include "Decl.h"

Decl::Decl(std::string identifier) {
    this->identifier = identifier;
}

bool Decl::equals(const Node &other) const {
    const Decl* o = dynamic_cast<const Decl*>(&other);
    if (o == nullptr) return false;

    return o->identifier == identifier;
}
