#ifndef DIG_IRGEN_BINARYEXPR_H
#define DIG_IRGEN_BINARYEXPR_H

#include "ir/TACFun.h"
#include "ir/TACOp.h"
#include "ast/BinaryExpr.h"

namespace Generate {
    TACOp binaryExpr(TACFun* env, BinaryExpr* e);
}

#endif
