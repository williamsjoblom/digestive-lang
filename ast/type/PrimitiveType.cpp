//
// Created by wax on 2/22/17.
//

#include <iostream>
#include <assert.h>
#include "PrimitiveType.h"

PrimitiveType::PrimitiveType(PrimitiveKind kind, int size) {
    this->kind = kind;
    this->sz = size;
}

void PrimitiveType::dump() const {
    switch (kind) {
        case PrimitiveKind::INTEGER:
            std::cout << "int";
            break;
        case PrimitiveKind::NATURAL:
            std::cout << "nat";
            break;
        default:
            assert(false);
    }

    std::cout << sz;
}

int PrimitiveType::byteSize() const {
    return sz;
}

bool PrimitiveType::isPrimitive() const {
    return true;
}

bool PrimitiveType::equals(const Node &other) const {
    const PrimitiveType* o = dynamic_cast<const PrimitiveType*>(&other);
    if (o == nullptr) return false;

    return sz == o->sz && kind == o->kind;
}
