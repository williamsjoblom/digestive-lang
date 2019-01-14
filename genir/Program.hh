#pragma once

#include "ir/TACProgram.hh"
#include "ir/TACFun.hh"
#include "ast/Unit.hh"

namespace Generate {
    void unit(TACProgram& program, Unit* unit);
}
