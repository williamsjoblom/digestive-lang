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

#include "ast/BinaryExpr.h"
#include "ast/LiteralExpr.h"

using namespace asmjit;

namespace Generate {

    X86GpVar* expression(X86Compiler &c, BinaryExpr* expr) {

        X86GpVar* left = expr->left->generate(c);
        X86GpVar* right = expr->right->generate(c);

        switch(expr->op->symbol) {
            case OperatorSymbol::PLUS:
                c.add(*left, *right);
                break;
            case OperatorSymbol::MINUS:
                c.sub(*left, *right);
                break;
            case OperatorSymbol::MUL:
                c.imul(*left, *right);
                break;
            case OperatorSymbol::DIV:
                assert(false); // Not implemented.
                break;
        }

        return left;
    }

    X86GpVar* expression(X86Compiler &c, LiteralExpr* expr) {
        X86GpVar* result = new X86GpVar(c.newInt32());
        c.mov(*result, expr->value);

        return result;
    }

    X86GpVar* expression(X86Compiler &c, VariableExpr* expr) {
        return expr->declaration->bVar;
    }

    X86GpVar* expression(X86Compiler &c, FunctionCall* expr) {
        X86GpVar ret = c.newInt32();

        X86GpVar handle = c.newIntPtr();
        c.mov(handle, imm_ptr(JitContext::handles + expr->declaration->bHandleIndex));

        X86CallNode* call = c.addCall(x86::ptr(handle), FuncBuilder0<int>(kCallConvHost));
        //X86CallNode* call = c.addCall(expr->declaration->bEntryLabel, FuncBuilder0<int>(kCallConvHost));
        call->setRet(0, ret);

        return new X86GpVar(ret);
    }
}
