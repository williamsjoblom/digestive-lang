//
// Created by wax on 12/15/16.
//

#include "Parse.h"
#include "ParseError.h"
#include "ast/FunctionDecl.h"
#include "lexer/TokenQueue.h"

namespace Parse {
    FunctionDecl* function(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("func")) return nullptr;

        Token identifier = tokens.expect(IDENTIFIER);
        BlockStmt* body = Parse::block(tokens);

        if (body == nullptr) {
            Token t = tokens.top();
            parseError(t, "Expected function body");
        }

        return new FunctionDecl(identifier.value, body);
    }
}