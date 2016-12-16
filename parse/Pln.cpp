//
// Created by wax on 12/16/16.
//

#include "ast/PlnStmt.h"
#include "lexer/TokenQueue.h"
#include "Parse.h"

namespace Parse {
    PlnStmt* pln(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("pln")) return nullptr;
        Expr* expr = Parse::expression(tokens);
        tokens.expect(SEMICOL);
        return new PlnStmt(expr);
    }
}