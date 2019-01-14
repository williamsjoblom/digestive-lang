#include "lexer/TokenQueue.hh"
#include "ast/LoopStmt.hh"
#include "Parse.hh"

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
