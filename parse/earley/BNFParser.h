#pragma once

/**
 * Forward declarations.
 */
struct BNFGrammar;
class TokenQueue;


namespace Earley {
    BNFGrammar parseGrammar(TokenQueue& tokens);
}
