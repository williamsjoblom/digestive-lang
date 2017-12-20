//
// Created by wax on 12/14/16.
//

#ifndef DIG_EXPR_H
#define DIG_EXPR_H

#include <asmjit/asmjit.h>

#include "type/DType.h"
#include "gen/Base.h"
#include "ast/type/Type.h"
#include "Node.h"

#include "ir/TACEnv.h"
#include "ir/TACOp.h"

using namespace asmjit;

class Expr : public Node {
public:
    DType type;

    virtual TACOp generate(TACEnv& env) = 0; 
    virtual Regs generate(X86Compiler &c) = 0;
};


#endif //DIG_EXPRESSION_H
