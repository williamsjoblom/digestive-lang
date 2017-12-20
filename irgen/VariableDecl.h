#pragma once

#include "ast/VariableDecl.h"
#include "ir/TACEnv.h"
#include "ir/TAC.h"

namespace Generate {
    void variableDeclaration(TACEnv& env, VariableDecl* decl);
}
