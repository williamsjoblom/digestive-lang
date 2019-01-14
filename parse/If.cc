#include "lexer/TokenQueue.hh"
#include "ast/IfStmt.hh"
#include "Parse.hh"

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
