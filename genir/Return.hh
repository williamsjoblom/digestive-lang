#pragma once

#include "ir/TACFun.hh"
#include "ast/ReturnStmt.hh"

namespace Generate {
    void returnStmt(TACFun* fun, ReturnStmt* stmt);
}
