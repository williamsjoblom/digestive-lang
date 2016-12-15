//
// Created by wax on 12/15/16.
//

#include <ast/ExprStmt.h>
#include "lexer/TokenQueue.h"

#include "ast/Stmt.h"
#include "ParseError.h"
#include "Parse.h"

namespace Parse {
    Stmt* statement(TokenQueue& tokens) {
        Token top = tokens.top();

        Stmt* stmt;
        if ((stmt = Parse::function(tokens)) != nullptr) return stmt;
        else return new ExprStmt(Parse::expression(tokens));
    }
}
