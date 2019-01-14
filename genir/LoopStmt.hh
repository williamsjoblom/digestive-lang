#pragma once

#include "ir/TACFun.hh"
#include "ast/LoopStmt.hh"

namespace Generate {
    void loopStmt(TACFun* env, LoopStmt* stmt);
}

