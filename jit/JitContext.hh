#pragma once

#include "ast/Unit.hh"

namespace JitContext {
    extern unsigned int handleCount;
    extern void** handles;
    extern Unit* root;

    void allocateHandles(unsigned int capacity);
    unsigned int addHandle(void* ptr);
    void setHandle(int index, void* ptr);
    void dumpHandles();
}
