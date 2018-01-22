#include "TACType.h"

#include <assert.h>
#include <sstream>
#include <asmjit/asmjit.h>

TACType::TACType() { }

TACType::TACType(DType& type) {
    if (type.isTuple()) {
	kind = TACKind::PTR;
	byteSize = sizeof(void*);
    } else if (type.isPrimitive()) {
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
    } else {
	assert(false);
    }

    ref = type.ref;
}


TACType::TACType(TACKind kind, int byteSize, bool ref) {
    this->kind = kind;
    this->byteSize = byteSize;
    this->ref = ref;
}


bool TACType::isSigned() {
    return kind == SIGNED;
}


bool TACType::isUnsigned() {
    return kind == UNSIGNED;
}


int TACType::asmjitId() {
    if (kind == TACKind::SIGNED) {
	switch (byteSize) {
	case 1: return asmjit::TypeId::kI8;
	case 2: return asmjit::TypeId::kI16;
	case 4: return asmjit::TypeId::kI32;
	case 8: return asmjit::TypeId::kI64;
	default: assert(false);
	}
    } else if (kind == TACKind::UNSIGNED) {
	switch (byteSize) {
	case 1: return asmjit::TypeId::kU8;
	case 2: return asmjit::TypeId::kU16;
	case 4: return asmjit::TypeId::kU32;
	case 8: return asmjit::TypeId::kU64;
	default: assert(false);
	}
    } else if (kind == TACKind::PTR) {
	return asmjit::TypeId::kIntPtr;
    }

    assert(false);
}


std::string TACType::toS() const {
    std::stringstream ss;
    switch (kind) {
    case PTR: ss << "p"; break;
    case SIGNED: ss << "i"; break;
    case UNSIGNED: ss << "u"; break;
    default: assert(false);
    }

    ss << byteSize * 8;
    
    return ss.str();
}
