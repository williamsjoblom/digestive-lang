//
// Created by wax on 6/27/17.
//

#include "TupleType.h"

TupleType::TupleType(std::vector<const Type *> types) {
    this->types = types;
}

int TupleType::byteSize() const {
    int sz = 0;
    for (const Type* t : types) sz += t->byteSize();
}

void TupleType::dump() const {
}

bool TupleType::equals(const Node &other) const {
    const TupleType* o = dynamic_cast<const TupleType*>(&other);
    if (o == nullptr) return false;

    if (types.size() != o->types.size()) return false;
    for (int i = 0; i < types.size(); i++)
        if (*types[i] != *o->types[i]) return false;

    return true;
}



