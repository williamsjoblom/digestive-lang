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

    X86Gp expression(X86Compiler &c, BinaryExpr* expr) {
        X86Gp left = expr->left->generate(c);
        X86Gp right = expr->right->generate(c);

        X86Gp result = c.newInt32("binexp");

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
                c.imul(result, right);
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

    X86Gp expression(X86Compiler &c, IntegerLiteral* expr) {
        X86Gp result = c.newInt32(("literal(" + std::to_string(expr->value) + ")").c_str());
        c.mov(result, expr->value);

        return result;
    }

    X86Gp expression(X86Compiler &c, VariableExpr* expr) {
        return expr->declaration->bVar;
    }

    X86Gp expression(X86Compiler &c, FunctionCall* expr) {
        FunctionDecl* decl = expr->declaration;

        X86Gp ret = c.newInt32("return");

        X86Gp handle = c.newGpd("handle");
        assert(decl->bHandleIndex != -1); // Handle index has not been assigned!
        c.mov(handle, imm_ptr(JitContext::handles + decl->bHandleIndex));

        std::vector<X86Gp> args;
        for (unsigned int i = 0; i < expr->arguments->size(); i++) {
            X86Gp a = expr->arguments->at(i)->generate(c);
            args.push_back(a);
        }

        CCFuncCall* call = c.call(x86::ptr(handle), decl->bGetFuncPrototype());
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
