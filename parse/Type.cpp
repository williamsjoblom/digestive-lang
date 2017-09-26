//
// Created by wax on 6/27/17.
//

#include "ast/type/DType.h"
#include "ast/type/Type.h"
#include "lexer/TokenQueue.h"
#include "ast/type/Types.h"
#include "ast/type/TupleType.h"

namespace Parse {

    static std::map<std::string, DType>  symbolToType = {
            {"bool", N8_TYPE},

            {"i16", I16_TYPE},
            {"i32", I32_TYPE},
            {"i64", I64_TYPE},

            {"n8", N8_TYPE},
            {"byte", N8_TYPE},
            {"n16", N16_TYPE},
            {"n32", N32_TYPE},
            {"n64", N64_TYPE},
    };

    DType type(TokenQueue& tokens) {
        Token t = tokens.top();
        if (t.type == IDENTIFIER) {
            auto p = symbolToType.find(t.value);
            if (p == symbolToType.end())
                return NIL_TYPE;

            tokens.pop();
            return p->second;
        } else if (t.type == LPAR) {
            tokens.pop(); // Pop LPAR.
	    
            if(tokens.eat(RPAR)) // () = nil-type.
                return NIL_TYPE;
	    
            std::vector<DType>* types = new std::vector<DType>();
            while (tokens.top().type == IDENTIFIER || tokens.top().type == LPAR) {

		// Check if this type has an associated label.
		std::string label = "";
		if (tokens.lookahead().type == COL) {
		    label = tokens.top().value;
		    tokens.pop();
		    tokens.eat(COL);
		}
		
                DType innerType = Parse::type(tokens);
		innerType.label = label;
                types->push_back(innerType);

                if(tokens.top().type != COMMA) break;
                tokens.pop();
	    }

            tokens.expect(RPAR); // Pop RPAR.

	    //assert(types != nullptr);
            if (types->size() == 0) return nullptr;
            if (types->size() == 1) return (*types)[0];
	    
            return DType(types);
        }

        return NIL_TYPE; // Return NIL-type.
    }
}
