//
// Created by wax on 12/14/16.
//

#ifndef DIG_STATEMENT_H
#define DIG_STATEMENT_H

#include <asmjit/asmjit.h>

#include "Node.h"
#include "Analyzable.h"
#include "ir/TACEnv.h"

using namespace asmjit;

class Stmt : public Node {
public:
    virtual void generate(X86Compiler &c) { };

    virtual void generate(TACEnv& env) { };
    virtual void analyze(Scope* scope) = 0;
};

#endif //DIG_STATEMENT_H
