#include "lexer/TokenQueue.hh"
#include "ast/Trap.hh"


namespace Parse {
    Trap* trap(TokenQueue& tokens) {
        if (!tokens.eatIdentifier("trap")) return nullptr;
        tokens.expect(SEMICOL);
        return new Trap();
    }
}
