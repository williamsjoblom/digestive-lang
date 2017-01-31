//
// Created by wax on 1/23/17.
//

#ifndef DIG_GEN_H
#define DIG_GEN_H

#include <ast/LoopStmt.h>
#include <ast/PlnStmt.h>
#include "ast/FunctionCall.h"
#include "asmjit/asmjit.h"
#include "ast/VariableExpr.h"
#include "ast/ReturnStmt.h"
#include "ast/Unit.h"
#include "ast/BinaryExpr.h"
#include "ast/LiteralExpr.h"
#include "ast/Expr.h"
#include "ast/BlockStmt.h"

typedef int (*ProgramType)(void);

using namespace asmjit;

namespace Generate {
    ProgramType program(JitRuntime* runtime, Unit* unit);
    void unit(JitRuntime* runtime, Unit* unit);
    void unit(X86Compiler &c, Unit* unit);

    void statement(X86Compiler &c, BlockStmt* stmt);
    void statement(X86Compiler &c, ReturnStmt* stmt);

    void loop(X86Compiler& c, LoopStmt* stmt);

    void function(X86Compiler &c, FunctionDecl* func); // Old "inline"
    void* function(JitRuntime* runtime, FunctionDecl* func);

    void variableDeclaration(X86Compiler &c, VariableDecl* decl);

    void pln(X86Compiler& c, PlnStmt* stmt);

    X86GpVar* expression(X86Compiler &c, BinaryExpr* expr);
    X86GpVar* expression(X86Compiler &c, LiteralExpr* expr);
    X86GpVar* expression(X86Compiler &c, VariableExpr* expr);
    X86GpVar* expression(X86Compiler &c, FunctionCall* expr);
}

#endif //DIG_GEN_H
