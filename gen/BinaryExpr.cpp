//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>
#include <assert.h>
#include <ast/type/PrimitiveType.h>

#include "Gen.h"

using namespace asmjit;

X86Gp primitiveBinaryExpr(X86Compiler &c, BinaryExpr* expr) {
    const PrimitiveType* type = dynamic_cast<const PrimitiveType*>(expr->type);
    assert(type != nullptr);

    X86Gp left = Generate::cast(c, expr->left, type);
    X86Gp right = Generate::cast(c, expr->right, type);

    X86Gp result = Generate::typedRegister(c, type);

    switch(expr->op->symbol) {
        case OperatorSymbol::PLUS:
            c.mov(result, left);
            c.add(result, right);
            break;
        case OperatorSymbol::MINUS:
            c.mov(result, left);
            c.sub(result, right);
            break;
        case OperatorSymbol::MUL:
            c.mov(result, left);
            if (type->kind == INTEGER)
                c.imul(result, right);
            else
                c.mul(result, right);
            break;
        case OperatorSymbol::DIV:
            c.mov(result, left);
            assert(false); // Not implemented.
            break;

        case OperatorSymbol::EQ:
            c.cmp(left, right);
            c.sete(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::NOTEQ:
            c.cmp(left, right);
            c.setne(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::LESSEQ:
            c.cmp(left, right);
            c.setle(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::GREATEREQ:
            c.cmp(left, right);
            c.setge(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::LESS:
            c.cmp(left, right);
            c.setl(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::GREATER:
            c.cmp(left, right);
            c.setg(result.r8());
            c.movzx(result, result.r8());
            break;
        default:
            assert(false);
    }

    return result;
}

namespace Generate {

    X86Gp expression(X86Compiler &c, BinaryExpr* expr) {
        if (expr->left->type->isPrimitive() && expr->right->type->isPrimitive()) {
            return primitiveBinaryExpr(c, expr);
        }

        // Not returning before this point should have resulted in a semantic error during analysis of the binary expr.
        assert(false);
    }

}

