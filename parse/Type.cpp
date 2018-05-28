//
// Created by wax on 6/27/17.
//

#include "ast/type/DType.h"
#include "ast/type/Type.h"
#include "lexer/TokenQueue.h"
#include "ast/type/Types.h"
#include "ast/type/TupleType.h"
#include "parse/ParseError.h"
#include "options.h"

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
	bool ref = tokens.eat(AT);
	
        Token t = tokens.top();
	if (t.type == IDENTIFIER) {
	    if (ref) parseError(t, "Primitive reference types not allowed");
	    
            auto p = symbolToType.find(t.value);
            if (p == symbolToType.end())
                return NIL_TYPE;

            tokens.pop();
	    
	    DType type = p->second;
	    return type;
        } else if (t.type == LPAR) {
            tokens.pop(); // Pop LPAR.
	    
            if(tokens.eat(RPAR)) { // () = nil-type.
		if (ref) parseError(t, "nil reference types are not allowed");
                return NIL_TYPE;
	    }
	    
            std::vector<DType>* types = new std::vector<DType>();
            while (tokens.top().type == IDENTIFIER || tokens.top().type == LPAR) {

		// Check if this type has an associated label.
		std::string label = "";
		if (tokens.lookahead().type == COL) {
		    Token labelToken = tokens.top();
		    label = labelToken.value;
		    
		    tokens.pop();
		    tokens.eat(COL);

		    for (DType type : *types) {
			if (type.label == label)
			    parseError(labelToken,
				       "A labeled type with this name already exist in this tuple");
		    }
		}
		
                DType innerType = Parse::type(tokens);
		innerType.label = label;
                types->push_back(innerType);

                if(tokens.top().type != COMMA) break;
                tokens.pop();
	    }

            tokens.expect(RPAR); // Pop RPAR.

            if (types->size() == 0) {
		if (ref) parseError(t, "nil reference types are not allowed");
		return NIL_TYPE;
	    }
	    
            if (types->size() == 1) {
		DType primitive = (*types)[0];
		primitive.ref = ref;
		return primitive;
	    }

            return DType(types, ref);
        }

	if (ref) parseError(t, "nil reference types are not allowed");
        return NIL_TYPE; // Return NIL-type.
    }
}
