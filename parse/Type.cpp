//
// Created by wax on 6/27/17.
//

#include "ast/type/Type.h"
#include "lexer/TokenQueue.h"
#include "ast/type/Types.h"
#include "ast/type/TupleType.h"

namespace Parse {

    static std::map<std::string, const Type*>  symbolToType = {
            {"i16", &INT16TYPE},
            {"i32", &INT32TYPE},
            {"i64", &INT64TYPE},

            {"n8", &NAT8TYPE},
            {"byte", &NAT8TYPE},
            {"n16", &NAT16TYPE},
            {"n32", &NAT32TYPE},
            {"n64", &NAT64TYPE},
    };

    const Type* type(TokenQueue& tokens) {
        Token t = tokens.top();
        if (t.type == IDENTIFIER) {
            auto p = symbolToType.find(t.value);
            if (p == symbolToType.end())
                return nullptr;

            tokens.pop();
            return p->second;
        } else if (t.type == LPAR) {
            tokens.pop(); // Pop LPAR.

            std::vector<const Type*> types = std::vector<const Type*>();
            while (tokens.top().type == IDENTIFIER) {
                const Type* innerType = Parse::type(tokens);
                types.push_back(innerType);

                if(tokens.top().type != COMMA) break;
                tokens.pop();
            }

            tokens.pop(); // Pop RPAR.

            if (types.size() == 0) return nullptr;
            if (types.size() == 1) return types[0];
            return new TupleType(types);
        }
    }
}