#pragma once

#include "ast/FunctionCall.hh"
#include "ir/TACFun.hh"
#include "ir/TACOp.hh"

namespace Generate {
    TACOp functionCall(TACFun* fun, FunctionCall* call);
}
