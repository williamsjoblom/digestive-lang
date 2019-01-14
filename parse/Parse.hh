#pragma once

#include "ast/ReturnStmt.hh"
#include "ast/VariableDecl.hh"
#include "ast/FunctionCall.hh"
#include "ast/PlnStmt.hh"
#include "ast/IfStmt.hh"
#include "ast/Unit.hh"
#include "ast/LoopStmt.hh"
#include "ast/Expr.hh"
#include "ast/FunctionDecl.hh"
#include "ast/Trap.hh"
#include "lexer/TokenQueue.hh"


// TODO divide into several headers if compile times gets too slow

namespace Parse {
    BlockStmt* block(TokenQueue& tokens);
    BlockStmt* file(TokenQueue& tokens);
    Expr* expression(TokenQueue& tokens);
    FunctionDecl* function(TokenQueue& tokens);
    Stmt* statement(TokenQueue& tokens);
    ReturnStmt* ret(TokenQueue& tokens);
    VariableDecl* variable(TokenQueue& tokens);
    std::vector<Expr*>* argumentList(TokenQueue& tokens);
    std::vector<VariableDecl*>* parameterList(TokenQueue& tokens);
    PlnStmt* pln(TokenQueue& tokens);
    IfStmt* ifStmt(TokenQueue& tokens);
    LoopStmt* loopStmt(TokenQueue& tokens);
    Unit* unit(TokenQueue& tokens);
    Trap* trap(TokenQueue& tokens);
    DType type(TokenQueue& tokens);
}
