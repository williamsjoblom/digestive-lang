//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>
#include "ast/type/PrimitiveType.h"
#include "ast/Expr.h"
#include "Gen.h"

using namespace asmjit;


X86Gp primitiveCast(X86Compiler& c, Expr* expr, DType& type) {
    X86Gp e = expr->generate(c)[0];

    if (expr->type.byteSize() >= type.byteSize()) {
        // Target type smaller than expression type, no extend required.
        switch (type.byteSize()) {
            case 1: return e.r8();
            case 2: return e.r16();
            case 4: return e.r32();
            case 8: return e.r64();
            default: assert(false);
        }
    } else {
        // TODO: verify that these widening moves works as expected.
        X86Gp result = Generate::typedRegister(c, type)[0];

        if (type.type.primitive == DPrimitiveKind::INTEGER) {
            c.movsx(result, e);
        } else if (type.type.primitive == DPrimitiveKind::NATURAL) {
            c.movzx(result, e);
        }

        return result;
    }
}



namespace Generate {
    Regs cast(X86Compiler& c, Expr* expr, DType& type) {
        // Failed assertion should have resulted in semantic error.
        c.getLastError();

        if (expr->type == type) return expr->generate(c);

        assert(expr->type.isPrimitive());
        return { primitiveCast(c, expr, type) };
    }
}

