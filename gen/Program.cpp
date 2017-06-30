//
// Created by wax on 1/23/17.
//

#include <iostream>

#include "jit/JitContext.h"
#include "ast/Unit.h"
#include "config.h"
#include "Gen.h"

namespace Generate {

    void entry(X86Compiler& c, Unit* unit);
    void functions(JitRuntime* runtime, Unit* unit);

    ProgramType program(JitRuntime* runtime, Unit* unit) {
        CodeHolder code;
        code.init(runtime->getCodeInfo());

        X86Compiler c(&code);

        StringLogger logger;
        code.setLogger(&logger);

        Generate::functions(runtime, unit);

        Generate::entry(c, unit);

        c.finalize();

        ProgramType func;
        Error err = runtime->add(&func, &code);
        if (err) throw 1;

        std::cout << "entry:" << std::endl;
        std::cout << logger.getString();

        return func;
    }

    void entry(X86Compiler& c, Unit* unit) {
        CCFunc* f = c.addFunc(FuncSignature0<int>(CallConv::kIdHost));
        f->getFrameInfo().enableCalls();
        f->getFrameInfo().enablePreservedFP();

        unit->statements->generate(c);

        c.endFunc();
    }

    void functions(JitRuntime* runtime, Unit* unit) {

        for (FunctionDecl* func : unit->functions) {
            func->bHandleIndex = JitContext::handleCount;

            void* ptr = Generate::function(runtime, func);

            int index = JitContext::addHandle(ptr);
            assert(index == func->bHandleIndex);
        }
    }
}