#pragma once

#include "asmjit/asmjit.h"

#include "Node.hh"
#include "Analyzable.hh"

class TACFun;

using namespace asmjit;

class Stmt : public Node {
public:
    virtual void generate(TACFun* env) { };
    virtual void analyze(Scope* scope) = 0;
};

