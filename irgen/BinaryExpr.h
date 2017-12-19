#ifndef DIG_IRGEN_BINARYEXPR_H
#define DIG_IRGEN_BINARYEXPR_H

#include "ir/TACEnv.h"
#include "ir/TACOp.h"
#include "ast/BinaryExpr.h"

namespace Generate {
    TACOp binaryExpr(TACEnv& env, BinaryExpr* e);
}

#endif
