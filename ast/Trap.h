//
// Created by wax on 2/17/17.
//

#ifndef DIG_DBGBRK_H
#define DIG_DBGBRK_H

#include "Stmt.h"

class Trap : public Stmt {
public:
    void analyze(Scope* scope) { }
    void generate(X86Compiler& c) { c.int3(); }

    bool equals(const Node& other) const { return true; }
    void dump(size_t indent) { }
};

#endif //DIG_DBGBRK_H
