#pragma once

#include "ir/TACFun.h"
#include "ast/LoopStmt.h"

namespace Generate {
    void loopStmt(TACFun* env, LoopStmt* stmt);
}

