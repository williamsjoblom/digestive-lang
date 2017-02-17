//
// Created by wax on 12/15/16.
//

#include <ast/ExprStmt.h>
#include "lexer/TokenQueue.h"

#include "Parse.h"

namespace Parse {
    Stmt* statement(TokenQueue& tokens) {
        Token top = tokens.top();

        Stmt* stmt;
        if((stmt = Parse::ret(tokens)) != nullptr) return stmt;
        else if((stmt = Parse::block(tokens)) != nullptr) return stmt;
        else if((stmt = Parse::variable(tokens)) != nullptr) return stmt;
        else if((stmt = Parse::pln(tokens)) != nullptr) return stmt;
        else if((stmt = Parse::ifStmt(tokens)) != nullptr) return stmt;
        else if((stmt = Parse::loopStmt(tokens)) != nullptr) return stmt;
        else if((stmt = Parse::trap(tokens)) != nullptr) return stmt;

        Expr* expression = Parse::expression(tokens);
        tokens.expect(SEMICOL);
        return new ExprStmt(expression);
    }
}
