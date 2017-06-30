//
// Created by wax on 12/15/16.
//

#include "ast/type/Types.h"
#include "Parse.h"
#include "ParseError.h"
#include "ast/FunctionDecl.h"
#include "lexer/TokenQueue.h"

/*
 * func <ID>([<DECL>[, <DECL>]*]?) -> <TYPE>  <BLOCK>
 *
 */

namespace Parse {

    FunctionDecl* function(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("func")) return nullptr;

        Token identifier = tokens.expect(IDENTIFIER);
        std::vector<VariableDecl*>* parameters = Parse::parameterList(tokens);


        const Type* returnType;
        if(tokens.eat(RIGHTARROW)) {
            returnType = Parse::type(tokens);
            if(returnType == nullptr) {
                Token t = tokens.top();
                parseError(t, "Expected function return type");
            }
        } else {
            returnType = &VOIDTYPE;
        }

        bool dumpAssembly = false;
        while (tokens.top().type == TokenType::IDENTIFIER) {
            if (tokens.top().value == "dumpasm") {
                tokens.pop();
                dumpAssembly = true;
            }
        }

        BlockStmt* body = Parse::block(tokens);
        if (body == nullptr) {
            Token t = tokens.top();
            parseError(t, "Expected function body");
        }


        return new FunctionDecl(identifier.value, parameters, body, returnType, dumpAssembly);
    }

    VariableDecl* parameter(TokenQueue& tokens) {
        Token identifier = tokens.top();
        if (identifier.type != TokenType::IDENTIFIER) return nullptr;
        tokens.pop();

        tokens.expect(TokenType::COL);

        const Type* type = Parse::type(tokens);
        if (type == nullptr) {
            Token t = tokens.top();
            parseError(t, "Expected function return type");
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