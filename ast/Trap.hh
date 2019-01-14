#pragma once

#include "Stmt.hh"
#include "genir/Trap.hh"

class Trap : public Stmt {
public:
    void analyze(Scope* scope) { }
    void generate(X86Compiler& c) { c.int3(); }
    void generate(TACFun* fun) { Generate::trap(fun, this); };

    bool equals(const Node& other) const { return true; }
    void dump(size_t indent) { }
};
