//
// Created by wax on 12/14/16.
//

#ifndef DIG_EXPR_H
#define DIG_EXPR_H

#include <asmjit/asmjit.h>

#include "Node.h"

using namespace asmjit;

class Expr : public Node {
public:
    virtual X86GpVar* generate(X86Compiler &c) { return new X86GpVar(); } // TODO: Make abstract.
};


#endif //DIG_EXPRESSION_H
