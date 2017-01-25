//
// Created by wax on 1/24/17.
//

#include "Gen.h"

namespace Generate {

    void function(X86Compiler &c, FunctionDecl* func) {
        FuncPrototype prototype = FuncBuilder0<int>(kCallConvHost);

        X86FuncNode* node = c.addFunc(prototype);
        func->bEntryLabel = node->getEntryLabel();

        func->body->generate(c);
        c.endFunc();
    }

    void* function(JitRuntime* runtime, FunctionDecl* func) {
        X86Assembler a(runtime);
        X86Compiler c(&a);

        FuncPrototype prototype = FuncBuilder0<int>(kCallConvHost);

        c.addFunc(prototype);
        func->body->generate(c);
        c.endFunc();

        c.finalize();

        return a.make();
    }
}