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

        FuncSignatureX prototype = func->bGetFuncPrototype();

        CCFunc* f = c.addFunc(prototype);
        f->getFrameInfo().enablePreservedFP();
        f->getDetail().addUsedRegs(x86::rbx.getKind(), Utils::mask(x86::rbx.getId()));

        c.newIntPtr();

        for (unsigned int i = 0; i < func->parameters->size(); i++) {
            VariableDecl* param = func->parameters->at(i);
            param->generate(c);

            c.setArg(i, param->bVar);
        }

        func->body->generate(c);

        c.endFunc();

        c.finalize();

        std::cout << "Generated func: " << std::endl << logger.getString();

        func->codeSize = code.getCodeSize();

        void* ptr;
        runtime->_add(&ptr, &code);

        return ptr;
    }
}