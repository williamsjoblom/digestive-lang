#pragma once

#include <string>
#include <unordered_set>

/**
 * Forward declarations.
 */
class Expr;
class BNFGrammar;
class TokenQueue;
class EState;


namespace Earley {
    /**
     * Parse.
     */
    bool parse(BNFGrammar& g, std::string rule, TokenQueue& tokens);
}
