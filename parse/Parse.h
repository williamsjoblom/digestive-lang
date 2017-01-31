//
// Created by wax on 12/15/16.
//

#ifndef DIG_PARSE_H
#define DIG_PARSE_H

#include <ast/ReturnStmt.h>
#include <ast/VariableDecl.h>
#include <ast/FunctionCall.h>
#include <ast/PlnStmt.h>
#include <ast/IfStmt.h>
#include <ast/Unit.h>
#include <ast/LoopStmt.h>
#include "ast/Expr.h"
#include "ast/FunctionDecl.h"
#include "lexer/TokenQueue.h"

// TODO: divide into several headers if compile times gets too slow

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
}

#endif //DIG_PARSE_H
