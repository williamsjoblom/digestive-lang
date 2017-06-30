//
// Created by wax on 6/30/17.
//

#include "DType.h"


DType::DType(const DType &o) {
    kind = o.kind;
    type = o.type;
    sz = o.sz;
}

DType::DType(DKind kind, int sz) {
    this->kind = kind;
}

DType::DType(DPrimitiveKind primitiveKind, int sz) {
    this->kind = DKind::PRIMITIVE;
    this->type.primitive = primitiveKind;
    this->sz = sz;
}

DType::~DType() { }

int DType::byteSize() const {
    return sz;
}






