#pragma once

#include "asmjit/asmjit.h"

#include "type/DType.hh"
#include "ast/type/Type.hh"
#include "Node.hh"

using namespace asmjit;

/**
 * Forward declarations.
 */
class TACOp;
class TACFun;

class Expr : public Node {
public:
    DType type;

    virtual TACOp generate(TACFun* env) = 0; 
};
