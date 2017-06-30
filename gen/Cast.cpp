//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>
#include "ast/type/PrimitiveType.h"
#include "ast/type/TupleType.h"
#include "ast/Expr.h"
#include "Gen.h"

using namespace asmjit;

class CastVisitor : TypeVisitor {
public:
    X86Compiler* c;
    Expr* expr;
    X86Gp result;

    X86Gp generateCast(X86Compiler* c, Expr* expr, const Type* toType);
    void visit(const PrimitiveType* type) override;
    void visit(const TupleType* type) override;
};


void CastVisitor::visit(const PrimitiveType* type) {
    // Failed assertion should have resulted in semantic error.
    assert(expr->type->isPrimitive());


    X86Gp e = expr->generate(*c);

    if (expr->type->byteSize() >= type->byteSize()) {
        // Target type smaller than expression type, no extend required.
        switch (type->byteSize()) {
            case 1: result = e.r8(); break;
            case 2: result = e.r16(); break;
            case 4: result = e.r32(); break;
            case 8: result = e.r64(); break;
            default: assert(false);
        }
    } else {
        // TODO: verify that these widening moves works as expected.
        result = Generate::typedRegister(*c, type);

        if (type->kind == INTEGER) {
            c->movsxd(result, e);
        } else if (type->kind == NATURAL) {
            c->movzx(result, e);
        }
    }
}

void CastVisitor::visit(const TupleType* type) {
    if ((*type).equals(*expr->type)) result = expr->generate(*c);
    //assert(false); //Not implemented
}

X86Gp CastVisitor::generateCast(X86Compiler* c, Expr* expr, const Type* toType) {
    this->c = c;
    this->expr = expr;

    toType->accept(this);

    return result;
}

namespace Generate {

    X86Gp cast(X86Compiler& c, Expr* expr, const Type* t) {
        CastVisitor cv;
        cv.generateCast(&c, expr, t);
        return cv.result;
    }

}

