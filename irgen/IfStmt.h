#pragma once

#include "ir/TACEnv.h"
#include "ast/IfStmt.h"

namespace Generate {
    void ifStmt(TACEnv& env, IfStmt* stmt);
}

