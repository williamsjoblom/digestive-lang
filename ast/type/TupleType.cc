#include "TupleType.hh"


TupleType::TupleType(std::vector<const Type *> types) : types{types} { }


int TupleType::byteSize() const {
    int sz { 0 };
    for (const Type* t : types) sz += t->byteSize();
    return sz;
}


void TupleType::dump() const {
    // Not implemented.
}


bool TupleType::equals(const Node &other) const {
    const TupleType* o { dynamic_cast<const TupleType*>(&other) };
    if (o == nullptr) return false;

    if (types.size() != o->types.size()) return false;
    for (int i { 0 }; i < types.size(); i++)
        if (*types[i] != *o->types[i]) return false;

    return true;
}



