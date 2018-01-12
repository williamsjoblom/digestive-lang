#pragma once

#include "ast/VariableDecl.h"
#include "ir/TACFun.h"
#include "ir/TAC.h"

namespace Generate {
    void variableDeclaration(TACFun* env, VariableDecl* decl);
}
