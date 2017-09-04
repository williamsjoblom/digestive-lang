//
// Created by wax on 1/23/17.
//

#include <asmjit/asmjit.h>
#include <ast/VariableExpr.h>
#include <assert.h>
#include <ast/VariableDecl.h>
#include <ast/FunctionCall.h>
#include <ast/FunctionDecl.h>
#include <jit/JitContext.h>
#include <ast/IntegerLiteral.h>

#include "Gen.h"
#include "ast/BinaryExpr.h"
#include "ast/Literal.h"

using namespace asmjit;

namespace Generate {


    Regs expression(X86Compiler &c, IntegerLiteral* expr) {
        Regs regs = Generate::typedRegister(c, expr->type);
        assert(regs.size() == 1);
        c.mov(regs[0], Imm(expr->value));
        return regs;
    }

    Regs expression(X86Compiler &c, VariableExpr* expr) {
        std::cout << "bVar.size: " << expr->declaration->bVar.size() << std::endl;

        std::cout << expr->declaration->bVar[0].kIdAx;

        return expr->declaration->bVar;
    }

    std::vector<X86Gp> expression(X86Compiler &c, FunctionCall* expr) {
        FunctionDecl* decl = expr->declaration;

        X86Gp handle = c.newGpd("handle");
        assert(decl->bHandleIndex != -1); // Handle index has not been assigned!
        c.mov(handle, imm_ptr(JitContext::handles + decl->bHandleIndex));

        std::vector<X86Gp> args;
        for (unsigned int i = 0; i < expr->arguments->size(); i++) {
            Regs a = expr->arguments->at(i)->generate(c);
            assert(a.size() == 1);
            args.push_back(a[0]);
        }

        CCFuncCall* call = c.call(x86::ptr(handle), decl->bCreatePrototype());

        Regs ret = Generate::typedRegister(c, decl->returnType);
        if (expr->type.isPrimitive() && !expr->type.isNilType()) {
            // Primitives uses calling convention...
            call->setRet(0, ret[0]);
        } else if (expr->type.isTuple()) {
            // ...while tuples are passed on the stack.
            for (int i = (int) ret.size() - 1; i >= 0; i--) {
                 c.pop(ret[i]);
            }
        }

        for (unsigned int i = 0; i < args.size(); i++) {
            X86Gp a = args.at(i);
            call->setArg(i, a);
        }

        return ret;
    }

    Regs expression(X86Compiler& c, UnaryExpr* expr) {
        assert(expr->type.isPrimitive());

        Regs regs = expr->expr->generate(c);
        assert(regs.size() == 1);
        X86Gp value = regs[0];
        X86Gp result = c.newInt32("unaryexp");

        c.mov(result, value);
        if (expr->op->symbol == OperatorSymbol::MINUS)
            c.neg(result);

        return { result };
    }

    Regs expression(X86Compiler& c, TupleExpr* expr) {
        Regs regs = Generate::typedRegister(c, expr->type);

        for (int i = 0; i < regs.size(); i++) {
            Regs valueRegs = Generate::cast(c, expr->values[i], (*expr->type.type.tuple)[i]);
            assert(valueRegs.size() == 1);

            c.mov(regs[i], valueRegs[0]);
        }

        return regs;
    }

}
