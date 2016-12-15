//
// Created by wax on 12/15/16.
//

#include "ast/ReturnStmt.h"
#include "lexer/TokenQueue.h"
#include "Parse.h"

namespace Parse {
    ReturnStmt* ret(TokenQueue& tokens) {
        if(!tokens.eatIdentifier("return")) return nullptr;
        Expr* expression = Parse::expression(tokens);
        tokens.expect(SEMICOL);
        return new ReturnStmt(expression);
    }
}