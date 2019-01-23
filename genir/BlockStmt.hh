#pragma once

#include "ast/BlockStmt.hh"

class TACFun;

namespace Generate {
    void blockStmt(TACFun* env, BlockStmt* stmt);
}
