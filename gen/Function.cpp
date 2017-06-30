//
// Created by wax on 1/24/17.
//

#include "Gen.h"

namespace Generate {

    void function(X86Compiler &c, FunctionDecl* func) {
        FuncSignature prototype = FuncSignature0<int>(CallConv::kIdHost);

        CCFunc* f = c.addFunc(prototype);
        f->getFrameInfo().enablePreservedFP();
        f->getFrameInfo().enableCalls();


        func->body->generate(c);
        c.endFunc();
    }

    void* function(JitRuntime* runtime, FunctionDecl* func) {
        CodeHolder code;
        code.init(runtime->getCodeInfo());

        X86Compiler c(&code);
        StringLogger logger;
        code.setLogger(&logger);

        FuncSignatureX prototype = func->bCreatePrototype();

        CCFunc* f = c.addFunc(prototype);
        f->getFrameInfo().enablePreservedFP();

        for (unsigned int i = 0; i < func->parameters->size(); i++) {
            VariableDecl* param = func->parameters->at(i);
            param->generate(c);

            c.setArg(i, param->bVar);
        }

        func->body->generate(c);

        c.endFunc();

        c.finalize();

        if (func->dumpAssembly) {
            std::cout << func->identifier << "(): " << std::endl << logger.getString() << std::endl;
        }

        func->codeSize = code.getCodeSize();

        void* ptr;
        runtime->_add(&ptr, &code);

        return ptr;
    }
}