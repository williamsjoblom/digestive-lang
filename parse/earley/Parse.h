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
class ASTNode;


namespace Earley {
    /**
     * Parse.
     */
    ASTNode* parse(BNFGrammar& g, std::string topRule, TokenQueue& tokens);
}
