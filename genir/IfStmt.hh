#pragma once

#include "ir/TACFun.hh"
#include "ast/IfStmt.hh"

namespace Generate {
    void ifStmt(TACFun* env, IfStmt* stmt);
}

