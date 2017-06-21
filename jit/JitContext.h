//
// Created by wax on 1/25/17.
//

#ifndef DIG_JITCONTEXT_H
#define DIG_JITCONTEXT_H

#include <ast/Unit.h>

namespace JitContext {
    extern unsigned int handleCount;
    extern void** handles;
    extern Unit* root;

    void allocateHandles(unsigned int capacity);
    unsigned int addHandle(void* ptr);
    void setHandle(int index, void* ptr);
    void dumpHandles();
}

#endif //DIG_JITCONTEXT_H
