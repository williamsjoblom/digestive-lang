#include "lexer/TokenQueue.hh"
#include "ast/VariableDecl.hh"
#include "Parse.hh"
#include "ParseError.hh"

namespace Parse {
    VariableDecl* variable(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("var")) return nullptr;
        Token identifier = tokens.expect(IDENTIFIER);

        Expr* expression = nullptr;

        tokens.expect(TokenType::COL);

        DType type = Parse::type(tokens);

        if(tokens.eat(ASSIGN)) {
            expression = Parse::expression(tokens);
        } else if (type.isNilType()) {
            Token t = tokens.top();
            parseError(t, "Expected variable type or inferred type");
        }

        tokens.expect(SEMICOL);

        return new VariableDecl(identifier.value, type, expression);
    }
}
