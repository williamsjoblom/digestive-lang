#pragma once

#include "ast/VariableDecl.hh"
#include "ir/TACFun.hh"
#include "ir/TAC.hh"

namespace Generate {
    void variableDeclaration(TACFun* env, VariableDecl* decl);
}
