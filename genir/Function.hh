#pragma once

#include "ir/TACFun.hh"
#include "ast/FunctionDecl.hh"


namespace Generate {
    void function(TACFun* env, FunctionDecl* function);
}
