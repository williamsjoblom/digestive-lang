#include "ast/ReturnStmt.hh"
#include "lexer/TokenQueue.hh"
#include "Parse.hh"

namespace Parse {
    ReturnStmt* ret(TokenQueue& tokens) {
        if(!tokens.eatIdentifier("return")) return nullptr;
        Expr* expression = Parse::expression(tokens);
        tokens.expect(SEMICOL);
        return new ReturnStmt(expression);
    }
}
