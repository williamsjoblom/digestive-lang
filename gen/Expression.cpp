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


    X86Gp expression(X86Compiler &c, IntegerLiteral* expr) {
        X86Gp result = Generate::typedRegister(c, expr->type);
        c.mov(result, Imm(expr->value));
        return result;
    }

    X86Gp expression(X86Compiler &c, VariableExpr* expr) {
        return expr->declaration->bVar;
    }

    X86Gp expression(X86Compiler &c, FunctionCall* expr) {
        FunctionDecl* decl = expr->declaration;

        X86Gp ret = Generate::typedRegister(c, decl->returnType);

        X86Gp handle = c.newGpd("handle");
        assert(decl->bHandleIndex != -1); // Handle index has not been assigned!
        c.mov(handle, imm_ptr(JitContext::handles + decl->bHandleIndex));

        std::vector<X86Gp> args;
        for (unsigned int i = 0; i < expr->arguments->size(); i++) {
            X86Gp a = expr->arguments->at(i)->generate(c);
            args.push_back(a);
        }

        CCFuncCall* call = c.call(x86::ptr(handle), decl->bCreatePrototype());
        call->setRet(0, ret);


        for (unsigned int i = 0; i < args.size(); i++) {
            X86Gp a = args.at(i);
            call->setArg(i, a);
        }

        return ret;
    }

    X86Gp expression(X86Compiler& c, UnaryExpr* expr) {
        X86Gp value = expr->expr->generate(c);
        X86Gp result = c.newInt32("unaryexp");

        c.mov(result, value);
        c.neg(result);

        return result;
    }

}
