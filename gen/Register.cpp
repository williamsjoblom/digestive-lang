//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>
#include <assert.h>
#include <iostream>
#include "ast/type/Type.h"
#include "ast/type/PrimitiveType.h"

using namespace asmjit;


class TypedRegisterVisitor : TypeVisitor {
public:
    X86Compiler* c;
    const Type* type;
    X86Gp result;

    X86Gp generateRegister(X86Compiler* c, const Type* type);
    void visit(const PrimitiveType* type) override;
    void visit(const TupleType* type) override;
};


void TypedRegisterVisitor::visit(const PrimitiveType* type) {
    assert(type->kind != PrimitiveKind::VOID);

    switch (type->kind) {
        case PrimitiveKind::INTEGER:
            if (type->byteSize() == 2) { result = c->newInt16(); return; }
            if (type->byteSize() == 4) { result = c->newInt32(); return; }
            if (type->byteSize() == 8) { result = c->newInt64(); return; }
            break;
        case PrimitiveKind::NATURAL:
            if (type->byteSize() == 1) { result = c->newUInt8(); return; }
            if (type->byteSize() == 2) { result = c->newUInt16(); return; }
            if (type->byteSize() == 4) { result = c->newUInt32(); return; }
            if (type->byteSize() == 8) { result = c->newUInt64(); return; }
            break;
        default:
            assert(false);
    }

    assert(false); // Unsupported type size.
}

void TypedRegisterVisitor::visit(const TupleType *type) {
    // Typed registers should not be generated for a tuple.
    // Code generator should generate one typed reg for each tuple value.
    assert(false);
}

X86Gp TypedRegisterVisitor::generateRegister(X86Compiler* c, const Type* type) {
    this->c = c;
    this->type = type;

    type->accept(this);

    return result;
}

namespace Generate {

    X86Gp typedRegister(X86Compiler& c, const Type* t) {
        TypedRegisterVisitor v;
        return v.generateRegister(&c, t);
    }

}