#include "ast/type/Types.hh"
#include "Parse.hh"
#include "ParseError.hh"
#include "ast/FunctionDecl.hh"
#include "lexer/TokenQueue.hh"

/*
 * fun <ID>([<DECL>[, <DECL>]*]?) -> <TYPE>  <BLOCK>
 *
 */

namespace Parse {

    FunctionDecl* function(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("fun")) return nullptr;

        Token identifier = tokens.expect(IDENTIFIER);
        std::vector<VariableDecl*>* parameters = Parse::parameterList(tokens);

        DType returnType;
        if(tokens.eat(RIGHTARROW)) {
            returnType = Parse::type(tokens);
	} else {
            returnType = NIL_TYPE;
        }

        bool dumpAssembly = false;
	bool dumpIr = false;
        while (tokens.top().type == TokenType::IDENTIFIER) {
	    std::string value = tokens.top().value;
	    if (value == "dumpasm") {
		tokens.pop();
		dumpAssembly = true;
	    } else if (value == "dumpir") {
		tokens.pop();
		dumpIr = true;
	    }
	}

	BlockStmt* body = Parse::block(tokens);
        if (body == nullptr) {
            Token t = tokens.top();
            parseError(t, "Expected function body");
        }


        return new FunctionDecl(identifier.value, parameters, body, returnType, dumpAssembly, dumpIr);
    }

    VariableDecl* parameter(TokenQueue& tokens) {
        Token identifier = tokens.top();
        if (identifier.type != TokenType::IDENTIFIER) return nullptr;
        tokens.pop();

        tokens.expect(TokenType::COL);

        const DType type = Parse::type(tokens);
        if (type.isNilType()) {
            Token t = tokens.top();
            parseError(t, "Expected parameter type");
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
