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

    VariableDecl* parameter(TokenQueue& tokens) {
        Token identifier = tokens.top();
        if (identifier.type != TokenType::IDENTIFIER) return nullptr;
        tokens.pop();


        tokens.expect(COL);
        Type* type = Parse::type(tokens);
        if (type == nullptr) {
            Token t = tokens.top();
            parseError(t, "Expected type");
        }

        return new VariableDecl(identifier.value, type);
    }

    std::vector<VariableDecl*>* parameterList(TokenQueue& tokens) {
        std::vector<VariableDecl*>* parameters = new std::vector<VariableDecl*>();
        if (!tokens.eat(LPAR)) return nullptr;

        if (tokens.top().type != RPAR) {

            VariableDecl* parameter = Parse::parameter(tokens);
            while (parameter != nullptr) {
                parameters->push_back(parameter);
                tokens.eat(COMMA);
                parameter = Parse::parameter(tokens);
            }
        }

        tokens.expect(RPAR);

        return parameters;
    }
}