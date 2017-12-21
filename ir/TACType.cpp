
#include "TACType.h"

#include <assert.h>
#include <sstream>


TACType::TACType() { }

TACType::TACType(DType& type) {
    assert(type.isPrimitive());
    
    switch (type.type.primitive) {
    case DPrimitiveKind::INTEGER:
	kind = TACKind::SIGNED; break;
    case DPrimitiveKind::NATURAL:
	kind = TACKind::UNSIGNED; break;
    case DPrimitiveKind::NIL:
	kind = TACKind::PTR; break;
    default:
	assert(false);
    }

    byteSize = type.byteSize();
}

TACType::TACType(TACKind kind, int byteSize) {
    this->kind = kind;
    this->byteSize = byteSize;
}

bool TACType::isSigned() {
    return kind == SIGNED;
}

bool TACType::isUnsigned() {
    return kind == UNSIGNED;
}


std::string TACType::toS() const {
    std::stringstream ss;
    switch (kind) {
    case PTR: ss << "ptr"; break;
    case SIGNED: ss << "i"; break;
    case UNSIGNED: ss << "u"; break;
    default: assert(false);
    }

    ss << byteSize * 8;
    
    return ss.str();
}
