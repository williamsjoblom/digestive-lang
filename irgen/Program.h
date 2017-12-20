#ifndef DIG_IRGEN_PROGRAM_H
#define DIG_IRGEN_PROGRAM_H

#include "ir/TACEnv.h"
#include "ast/Unit.h"

namespace Generate {
    void unit(TACEnv& env, Unit* unit);
}

#endif
