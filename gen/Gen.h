//
// Created by wax on 1/23/17.
//

#ifndef DIG_GEN_H
#define DIG_GEN_H

#include "Base.h"

#include "ast/TupleExpr.h"
#include <ast/LoopStmt.h>
#include <ast/PlnStmt.h>
#include <ast/IfStmt.h>
#include <ast/UnaryExpr.h>
#include <ast/IntegerLiteral.h>
#include "ast/FunctionCall.h"
#include "asmjit/asmjit.h"
#include "ast/VariableExpr.h"
#include "ast/ReturnStmt.h"
#include <ast/Unit.h>
#include "ast/BinaryExpr.h"
#include "ast/Literal.h"
#include "ast/Expr.h"
#include "ast/BlockStmt.h"


using namespace asmjit;

class Unit;

namespace Generate {
    ProgramType program(JitRuntime* runtime, Unit* unit);
    void unit(JitRuntime* runtime, Unit* unit);
    void unit(X86Compiler &c, Unit* unit);

    void statement(X86Compiler &c, BlockStmt* stmt);
    void statement(X86Compiler &c, ReturnStmt* stmt);

    void loop(X86Compiler& c, LoopStmt* stmt);
    void ifStmt(X86Compiler& c, IfStmt* stmt);

    void function(X86Compiler &c, FunctionDecl* func); // Deprecated
    void* function(JitRuntime* runtime, FunctionDecl* func);

    void variableDeclaration(X86Compiler &c, VariableDecl* decl);

    void pln(X86Compiler& c, PlnStmt* stmt);

    X86Gp alloc(X86Compiler& c, size_t s);

    Regs cast(X86Compiler& c, Expr* expr, DType& t, bool destructive=false);
    Regs typedRegister(X86Compiler& c, DType& t);

    Regs expression(X86Compiler& c, BinaryExpr* expr);
    Regs expression(X86Compiler& c, IntegerLiteral* expr);
    Regs expression(X86Compiler& c, VariableExpr* expr);
    Regs expression(X86Compiler& c, FunctionCall* expr);
    Regs expression(X86Compiler& c, UnaryExpr* expr);
    Regs expression(X86Compiler& c, TupleExpr* expr);
}

#endif //DIG_GEN_H
