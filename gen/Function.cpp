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
        StringLogger logger;
        a.setLogger(&logger);

        FuncBuilderX prototype = FuncBuilderX(kCallConvHost);
        prototype.setRet(TypeId<int>::kId);

        for (unsigned int i = 0; i < func->parameters->size(); i++) {
            prototype.addArg(TypeId<int>::kId);
        }

        c.addFunc(prototype);

        for (unsigned int i = 0; i < func->parameters->size(); i++) {
            VariableDecl* param = func->parameters->at(i);
            param->generate(c);

            c.setArg(i, *param->bVar);
        }


        func->body->generate(c);



        c.endFunc();

        c.finalize();

        std::cout << "Generated func: " << std::endl << logger.getString();


        return a.make();
    }
}