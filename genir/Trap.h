#pragma once

#include "ir/TACFun.h"

/**
 * Forward declarations.
 */
class Trap;


namespace Generate {
    void trap(TACFun* fun, Trap* stmt);
}
