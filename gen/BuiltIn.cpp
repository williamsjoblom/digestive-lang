//
// Created by wax on 1/31/17.
//

#include "jit/BuiltIn.h"
#include "Gen.h"

namespace Generate {

    void pln(X86Compiler& c, PlnStmt* stmt) {
        Regs arg = stmt->expression->generate(c);
        assert(arg.size() == 1);
        assert(stmt->expression->type.isPrimitive());

        DPrimitiveKind pk = stmt->expression->type.type.primitive;
        int sz = stmt->expression->type.sz;

        CCFuncCall* call;
        if (pk == DPrimitiveKind::INTEGER && sz == 2)
            call = c.call(imm_ptr((void*) BuiltIn::pln<short>), FuncSignature1<void, short>(CallConv::kIdHost));
        else if (pk == DPrimitiveKind::INTEGER && sz == 4)
            call = c.call(imm_ptr((void*) BuiltIn::pln<int>), FuncSignature1<void, int>(CallConv::kIdHost));
        else if (pk == DPrimitiveKind::INTEGER && sz == 8)
            call = c.call(imm_ptr((void*) BuiltIn::pln<long>), FuncSignature1<void, long>(CallConv::kIdHost));
        else if (pk == DPrimitiveKind::NATURAL && sz == 1)
            call = c.call(imm_ptr((void*) BuiltIn::pln<char>), FuncSignature1<void, char>(CallConv::kIdHost));
        else if (pk == DPrimitiveKind::NATURAL && sz == 2)
            call = c.call(imm_ptr((void*) BuiltIn::pln<unsigned short>), FuncSignature1<void, unsigned short>(CallConv::kIdHost));
        else if (pk == DPrimitiveKind::NATURAL && sz == 4)
            call = c.call(imm_ptr((void*) BuiltIn::pln<unsigned int>), FuncSignature1<void, unsigned int>(CallConv::kIdHost));
        else if (pk == DPrimitiveKind::NATURAL && sz == 8)
            call = c.call(imm_ptr((void*) BuiltIn::pln<unsigned long>), FuncSignature1<void, unsigned long>(CallConv::kIdHost));
        else
            assert(false);

        call->setArg(0, arg[0]);
    }

}
