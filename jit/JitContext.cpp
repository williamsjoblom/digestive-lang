//
// Created by wax on 1/25/17.
//

#include <iostream>

namespace JitContext {
    void** handles = nullptr;

    // FIXME: Dump all handles, not only the first.
    void dumpHandles() {
        std::cout << "Handles: " << handles[0] << std::endl;
    }
}