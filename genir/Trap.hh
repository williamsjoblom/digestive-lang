#pragma once

#include "ir/TACFun.hh"

/**
 * Forward declarations.
 */
class Trap;

namespace Generate {
    /**
     * Generate TAC for Trap statement.
     */
    void trap(TACFun* fun, Trap* stmt);
}
