//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>
#include <assert.h>
#include <iostream>
#include <ast/type/DType.h>
#include "ast/type/Type.h"
#include "ast/type/PrimitiveType.h"
#include "Base.h"

using namespace asmjit;

namespace Generate {

    Regs typedRegister(X86Compiler& c, DType& type) {
        if (type.isPrimitive()) {
            assert(!type.isNilType());
            switch (type.type.primitive) {
                case DPrimitiveKind::INTEGER:
                    if (type.byteSize() == 2) { return {c.newInt16()}; }
                    if (type.byteSize() == 4) { return {c.newInt32()}; }
                    if (type.byteSize() == 8) { return {c.newInt64()}; }
                    break;
                case DPrimitiveKind::NATURAL:
                    if (type.byteSize() == 1) { return {c.newUInt8()}; }
                    if (type.byteSize() == 2) { return {c.newUInt16()}; }
                    if (type.byteSize() == 4) { return {c.newUInt32()}; }
                    if (type.byteSize() == 8) { return {c.newUInt64()}; }
                    break;
                default:
                    assert(false);
            }
        } else if (type.isTuple()){
            Regs regs = Regs();

            for (DType t : *type.type.tuple) {
                Regs valueRegs = Generate::typedRegister(c, t);
                assert(valueRegs.size() == 1);
                for (X86Gp valueReg : valueRegs)
                    regs.push_back(valueReg);
            }

            return regs;
        }

        assert(false);
    }

}