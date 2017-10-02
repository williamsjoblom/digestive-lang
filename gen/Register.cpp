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
	if (type.ref) {
	    return { c.newIntPtr() };
	} else if (type.isPrimitive()) {

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
                case DPrimitiveKind::NIL:
                    return { };
            }
        } else if (type.isTuple()){
            Regs regs = Regs();

            for (DType t : *type.type.tuple) {
                Regs valueRegs = Generate::typedRegister(c, t);
                
                for (X86Gp valueReg : valueRegs)
                    regs.push_back(valueReg);
            }

            return regs;
        }

        assert(false);
    }


    X86Mem constant(X86Compiler& c, DType& type, int64_t value, bool global) {
	assert(type.isPrimitive());

	int scope = global ? kConstScopeGlobal : kConstScopeLocal;
	
	switch (type.type.primitive) {
	case DPrimitiveKind::INTEGER:
	    if (type.byteSize() == 2) return c.newInt16Const(scope, (int16_t) value);
	    if (type.byteSize() == 4) return c.newInt32Const(scope, (int32_t) value);
	    if (type.byteSize() == 8) return c.newInt64Const(scope, (int64_t) value);
	    break;
	case DPrimitiveKind::NATURAL:
	    if (type.byteSize() == 1) return c.newByteConst(scope, (uint8_t) value);
	    if (type.byteSize() == 2) return c.newUInt16Const(scope, (uint16_t) value);
	    if (type.byteSize() == 4) return c.newUInt32Const(scope, (uint32_t) value);
	    if (type.byteSize() == 8) return c.newUInt64Const(scope, (uint64_t) value); 
	    break;
	case DPrimitiveKind::NIL:
	    assert(false);
	}
    }

}
