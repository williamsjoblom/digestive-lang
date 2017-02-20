//
// Created by wax on 1/31/17.
//

#include <jit/BuiltIn.h>
#include "Gen.h"

namespace Generate {

    void pln(X86Compiler& c, PlnStmt* stmt) {
        X86GpVar arg = stmt->expression->generate(c);

        X86CallNode* call = c.addCall(imm_ptr((void*) BuiltIn::pln), FuncBuilder1<void, int>(kCallConvHost));
        call->setArg(0, arg);
    }

}