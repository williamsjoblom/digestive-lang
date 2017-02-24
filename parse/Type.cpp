//
// Created by wax on 2/22/17.
//

#include "ast/type/Type.h"
#include "lexer/TokenQueue.h"

#include "ast/type/Types.h"

namespace Parse {

    static std::map<std::string, Type*> symbolToType =
            {
                    {"i8", &INT8TYPE},
                    {"i16", &INT16TYPE},
                    {"i32", &INT32TYPE},
                    {"i64", &INT64TYPE},

                    {"r32", &FLOAT32TYPE},
                    {"r64", &FLOAT64TYPE},
            };

    Type* type(TokenQueue& tokens) {
        Token t = tokens.top();
        if (t.type != IDENTIFIER) return nullptr;

        auto result = symbolToType.find(t.value);
        if (result != symbolToType.end()) {
            tokens.pop();
            return result->second;
        }

        return nullptr;
    }
}