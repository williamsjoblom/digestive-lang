//
// Created by wax on 2/22/17.
//

#include <iostream>
#include <assert.h>
#include "PrimitiveType.h"

PrimitiveType::PrimitiveType(PrimitiveKind kind, int size) {
    this->kind = kind;
    this->size = size;
}

void PrimitiveType::dump() const {
    switch (kind) {
        case PrimitiveKind::INTEGER:
            std::cout << "integer";
            break;
        case PrimitiveKind::REAL:
            std::cout << "real";
            break;
        default:
            assert(false);
    }

    std::cout << size;
}
