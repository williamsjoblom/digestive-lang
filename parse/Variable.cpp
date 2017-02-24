//
// Created by wax on 12/15/16.
//

#include "lexer/TokenQueue.h"
#include "ast/VariableDecl.h"
#include "Parse.h"

namespace Parse {
    VariableDecl* variable(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("var")) return nullptr;
        Token identifier = tokens.expect(IDENTIFIER);

        Expr* expression = nullptr;

        tokens.expect(COL);
        Type* type = Parse::type(tokens);

        if(tokens.eat(ASSIGN)) {
            expression = Parse::expression(tokens);
        }

        tokens.expect(SEMICOL);

        return new VariableDecl(identifier.value, type, expression);
    }
}