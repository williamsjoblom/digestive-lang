#pragma once

#include "ast/BlockStmt.h"

class TACFun;

namespace Generate {
    void blockStmt(TACFun* env, BlockStmt* stmt);
}
