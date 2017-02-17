//
// Created by wax on 2/17/17.
//

#include "lexer/TokenQueue.h"
#include "ast/Trap.h"

namespace Parse {
    Trap* trap(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("trap")) return nullptr;
        tokens.expect(SEMICOL);
        return new Trap();
    }
}