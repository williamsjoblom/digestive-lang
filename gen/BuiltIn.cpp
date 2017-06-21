//
// Created by wax on 1/31/17.
//

#include <jit/BuiltIn.h>
#include "Gen.h"

namespace Generate {

    void pln(X86Compiler& c, PlnStmt* stmt) {
        X86Gp arg = stmt->expression->generate(c);

        CCFuncCall* call = c.call(imm_ptr((void*) BuiltIn::pln), FuncSignature1<void, int>(CallConv::kIdHost));

        call->setArg(0, arg);
    }

}