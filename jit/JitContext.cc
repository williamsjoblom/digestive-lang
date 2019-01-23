#include <iostream>
#include <assert.h>

#include "ast/Unit.hh"

namespace JitContext {
    unsigned int handleCapacity = 0;
    unsigned int handleCount = 0;
    void** handles = nullptr;
    Unit* root = nullptr;

    void allocateHandles(unsigned int capacity) {
        handles = new void*[capacity];
        handleCapacity = capacity;
    }

    unsigned int addHandle(void* ptr) {
        if (handleCount >= handleCapacity) {
            std::cout << "Session function limit reached. Restart required." << std::endl;
            throw 1;
        }

        handleCount++;
        handles[handleCount - 1] = ptr;
        return handleCount - 1;
    }

    // FIXME Dump all handles, not only the first.
    void dumpHandles() {
        std::cout << "Handles: [ ";
        for (unsigned int i { 0 }; i < handleCount; i++) {
            std::cout << handles[i] << " ";
        }

        std::cout << "]" << std::endl;
    }

    void setHandle(unsigned int index, void *ptr) {
        if (index >= handleCount) assert(false); // Outside array bound.
        handles[index] = ptr;
    }
}
