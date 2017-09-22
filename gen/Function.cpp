//
// Created by wax on 1/24/17.
//

#include "Gen.h"
#include "ErrHandler.h"

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
	ErrHandler handler;
	code.setErrorHandler(&handler);

        X86Compiler c(&code);
        StringLogger logger;
        code.setLogger(&logger);

        FuncSignatureX prototype = func->bCreatePrototype();

        CCFunc* f = c.addFunc(prototype);
        f->getFrameInfo().enablePreservedFP();

	int argIndex = 0;
        for (unsigned int i = 0; i < func->parameters->size(); i++) {
            VariableDecl* param = func->parameters->at(i);
            param->generate(c);
            //assert(param->bVar.size() == 1);
	    for (X86Gp reg : param->bVar) {
		c.setArg(argIndex, reg);
		argIndex++;
	    }
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
