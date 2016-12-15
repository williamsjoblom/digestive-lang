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
        std::vector<VariableDecl*>* parameters = Parse::parameterList(tokens);
        BlockStmt* body = Parse::block(tokens);

        if (body == nullptr) {
            Token t = tokens.top();
            parseError(t, "Expected function body");
        }

        return new FunctionDecl(identifier.value, parameters, body);
    }

    std::vector<VariableDecl*>* parameterList(TokenQueue& tokens) {
        std::vector<VariableDecl*>* parameters = new std::vector<VariableDecl*>();
        if (!tokens.eat(LPAR)) return nullptr;

        if (tokens.top().type != RPAR) {
            VariableDecl* parameter = Parse::variable(tokens);
            while (parameter != nullptr) {
                parameters->push_back(parameter);
                tokens.eat(COMMA);
                parameter = Parse::variable(tokens);
            }
        }

        tokens.expect(RPAR);

        return parameters;
    }
}