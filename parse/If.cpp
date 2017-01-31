//
// Created by wax on 12/21/16.
//

#include <lexer/TokenQueue.h>
#include "ast/IfStmt.h"
#include "Parse.h"

namespace Parse {
    IfStmt* ifStmt(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("if")) return nullptr;

        tokens.expect(LPAR);
        Expr* condition = Parse::expression(tokens);
        tokens.expect(RPAR);

        Stmt* ifBody = Parse::statement(tokens);
        Stmt* elseBody = nullptr;
        if (tokens.eatIdentifier("else")) {
            elseBody = Parse::statement(tokens);
        }

        return new IfStmt(condition, ifBody, elseBody);
    }
}