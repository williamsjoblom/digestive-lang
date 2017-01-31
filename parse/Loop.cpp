//
// Created by wax on 1/31/17.
//

#include "lexer/TokenQueue.h"
#include "ast/LoopStmt.h"
#include "Parse.h"

namespace Parse {
    LoopStmt* loopStmt(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("loop")) return nullptr;

        tokens.expect(LPAR);
        Expr* condition = Parse::expression(tokens);
        tokens.expect(RPAR);

        Stmt* body = Parse::statement(tokens);

        return new LoopStmt(condition, body);
    }
}