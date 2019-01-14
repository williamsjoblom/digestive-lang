#include "ast/PlnStmt.hh"
#include "lexer/TokenQueue.hh"
#include "Parse.hh"


namespace Parse {
    PlnStmt* pln(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("pln")) return nullptr;
        Expr* expr = Parse::expression(tokens);
        tokens.expect(SEMICOL);
        return new PlnStmt(expr);
    }
}
