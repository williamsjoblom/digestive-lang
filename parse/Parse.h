//
// Created by wax on 12/15/16.
//

#ifndef DIG_PARSE_H
#define DIG_PARSE_H

#include "ast/Expr.h"
#include "ast/FunctionDecl.h"
#include "lexer/TokenQueue.h"
#include "ast/BlockStmt.h"

// TODO: divide into several headers if compile times gets too slow

namespace Parse {
    BlockStmt* block(TokenQueue& tokens);
    BlockStmt* file(TokenQueue& tokens);
    Expr* expression(TokenQueue& tokens);
    FunctionDecl* function(TokenQueue& tokens);
    Stmt* statement(TokenQueue& tokens);
}

#endif //DIG_PARSE_H
