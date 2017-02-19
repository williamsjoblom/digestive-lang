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

        X86GpVar* result = new X86GpVar(c.newInt32("binexp"));

        c.mov(*result, *left);

        switch(expr->op->symbol) {
            case OperatorSymbol::PLUS:
                c.add(*result, *right);
                break;
            case OperatorSymbol::MINUS:
                c.sub(*result, *right);
                break;
            case OperatorSymbol::MUL:
                c.imul(*result, *right);
                break;
            case OperatorSymbol::DIV:
                assert(false); // Not implemented.
                break;
        }

        return result;
    }

    X86GpVar* expression(X86Compiler &c, LiteralExpr* expr) {
        X86GpVar* result = new X86GpVar(c.newInt32(("literal(" + std::to_string(expr->value) + ")").c_str()));
        c.mov(*result, expr->value);

        return result;
    }

    X86GpVar* expression(X86Compiler &c, VariableExpr* expr) {
        return expr->declaration->bVar;
    }

    X86GpVar* expression(X86Compiler &c, FunctionCall* expr) {
        FunctionDecl* decl = expr->declaration;

        X86GpVar ret = c.newInt32("return");

        X86GpVar handle = c.newIntPtr("handle");
        c.mov(handle, imm_ptr(JitContext::handles + decl->bHandleIndex));

        std::vector<X86GpVar*> args;
        for (unsigned int i = 0; i < expr->arguments->size(); i++) {
            X86GpVar* a = expr->arguments->at(i)->generate(c);
            args.push_back(a);
        }

        X86CallNode* call = c.addCall(x86::ptr(handle), decl->bGetFuncPrototype());
        call->setRet(0, ret);


        for (unsigned int i = 0; i < args.size(); i++) {
            X86GpVar *a = args.at(i);
            call->setArg(i, *a);
        }

        //X86CallNode* call = c.addCall(expr->declaration->bEntryLabel, FuncBuilder0<int>(kCallConvHost));

        return new X86GpVar(ret);
    }
}
