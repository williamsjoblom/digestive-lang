#pragma once

#include "ast/FunctionCall.h"
#include "ir/TACFun.h"
#include "ir/TACOp.h"

namespace Generate {
    TACOp functionCall(TACFun* fun, FunctionCall* call);
}
