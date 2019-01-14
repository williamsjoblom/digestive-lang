#pragma once

#include "ir/TACFun.hh"
#include "ir/TACOp.hh"
#include "ast/BinaryExpr.hh"

namespace Generate {
    TACOp binaryExpr(TACFun* env, BinaryExpr* e);
}
