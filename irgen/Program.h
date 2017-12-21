#ifndef DIG_IRGEN_PROGRAM_H
#define DIG_IRGEN_PROGRAM_H

#include "ir/TACProgram.h"
#include "ir/TACFun.h"
#include "ast/Unit.h"

namespace Generate {
    void unit(TACProgram& program, Unit* unit);
}

#endif
