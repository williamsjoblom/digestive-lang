#pragma once

#include "ir/TACFun.h"
#include "ast/IfStmt.h"

namespace Generate {
    void ifStmt(TACFun* env, IfStmt* stmt);
}

