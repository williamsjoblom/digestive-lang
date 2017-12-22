#pragma once

#include "ir/TACFun.h"
#include "ast/ReturnStmt.h"

namespace Generate {
    void returnStmt(TACFun* fun, ReturnStmt* stmt);
}
