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
    virtual Regs generate(X86Compiler &c) = 0;
};


#endif //DIG_EXPRESSION_H
