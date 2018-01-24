#pragma once

/**
 * Forward declarations.
 */
class Expr;
class TokenQueue;


namespace Earley {
    Expr* parse(TokenQueue& tokens);
}
