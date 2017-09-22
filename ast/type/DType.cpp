//
// Created by wax on 6/30/17.
//

#include <assert.h>
#include <iostream>
#include "DType.h"

DType::DType() { }

DType::DType(const DType &o) {
   copy(o);
}

DType::DType(DKind kind, int sz) {
    this->kind = kind;
    this->sz = sz;
}

DType::DType(DPrimitiveKind primitiveKind, int sz) {
    this->kind = DKind::PRIMITIVE;
    this->type.primitive = primitiveKind;
    this->sz = sz;
}

DType::DType(std::vector<DType>* tupleTypes) {
    kind = DKind::TUPLE;
    type.tuple = tupleTypes;

    sz = 0;
    assert(tupleTypes != nullptr);
    for (DType t : *tupleTypes) sz += t.byteSize();
}


DType::~DType() {
    if (isSeq()) delete type.seq;
}

int DType::byteSize() const {
    return sz;
}

void DType::copy(const DType &o) {
    kind = o.kind;
    switch (kind) {
        case DKind::PRIMITIVE: type.primitive = o.type.primitive; break;
        case DKind::TUPLE: type.tuple = o.type.tuple; break;
        case DKind::ARRAY: type.seq = o.type.seq; break;
    }
    sz = o.sz;
}

bool DType::operator==(const DType &o) {
    if (kind != o.kind) return false;
    if (sz != o.sz) return false;



    switch (kind) {
        case DKind::PRIMITIVE: return type.primitive == o.type.primitive;
        case DKind::TUPLE:
            for (int i = 0; i < type.tuple->size(); i++)
                if ((*type.tuple)[i] != (*o.type.tuple)[i])
                    return false;
            return true;
        case DKind::ARRAY:  return type.seq == o.type.seq;
    }

    assert(false);
}

void DType::dump() const {
    switch(kind) {
        case DKind::PRIMITIVE:
            std::cout << "p";
            if (type.primitive == DPrimitiveKind::NATURAL)
                std::cout << "NAT";
            else if (type.primitive == DPrimitiveKind::INTEGER)
                std::cout << "INT";
            else if (type.primitive == DPrimitiveKind::NIL)
                std::cout << "NIL";
            break;
        case DKind::TUPLE:
            std::cout << "t( ";
            for (DType t : *type.tuple) {
                t.dump();
                std::cout << " ";
            }
            std::cout << ")";
            break;
        default:
            assert(false);
    }

    std::cout << sz;
}








