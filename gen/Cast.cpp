//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>

#include "ast/type/PrimitiveType.h"
#include "ast/Expr.h"
#include "Gen.h"
#include "gen/CallConv.h"

using namespace asmjit;

/**
 * "Cast" a register to type.
 */
X86Gp registerCast(X86Compiler& c, X86Gp reg, DType& regType, DType& targetType, bool destructive) {
    if (regType.byteSize() >= targetType.byteSize()) {
        // Target type smaller than expression type, no extend required.
	X86Gp result;
        switch (targetType.byteSize()) {
	case 1: result = reg.r8(); break;
	case 2: result = reg.r16(); break;
	case 4: result = reg.r32(); break;
	case 8: result = reg.r64(); break;
	default:
	    targetType.dump();
	    assert(false);
        }

	if (destructive) return result;
	
	X86Gp copy = c.newSimilarReg<X86Gp>(result, "copy");
	return copy;
    } else {
        // TODO: verify that these widening moves fully works as expected.
        X86Gp result = Generate::typedRegister(c, targetType)[0];

        if (targetType.type.primitive == DPrimitiveKind::INTEGER) {
            c.movsx(result, reg);
        } else if (targetType.type.primitive == DPrimitiveKind::NATURAL) {
            c.movzx(result, reg);
        }

        return result;
    }
}


/**
 * Cast expression to primitive.
 * Tuples can be casted to primitives, returning a primitive containing the tuples first value.
 */
X86Gp primitiveCast(X86Compiler& c, Regs regs, DType& fromType, DType& toType, bool destructive) {
    assert(fromType.isPrimitive() || fromType.isTuple());
    X86Gp e = regs[0];
    return registerCast(c, e, fromType, toType, destructive);
}


/**
 * Generate pointer dereference.
 */
Regs dereference(X86Compiler& c, X86Gp ptr, DType& type) {
    X86Mem mem = x86::ptr(ptr);    
    Regs result;
    
    std::vector<DType> flatType = flattenType(type);
    int offset = 0;
    for (int i = 0; i < flatType.size(); i++) {
	
	X86Gp sink = Generate::typedRegister(c, flatType[i])[0];
	mem.setOffset(offset);
	c.mov(sink, mem);
	result.push_back(sink);
	
	offset += flatType[i].byteSize();
    }
    
    return result;
}


/**
 * Cast expression to tuple.
 */
Regs tupleCast(X86Compiler& c, Regs regs, DType& fromType, DType& toType, bool destructive) {
    assert(fromType.isTuple());
    assert(fromType.type.tuple->size() >= toType.type.tuple->size());

    std::vector<DType> regTypes = flattenType(fromType);
    std::vector<DType> targetTypes = flattenType(toType);
    assert(regTypes.size() >= targetTypes.size());
    
    Regs result = Regs();
    for (int i = 0; i < regs.size(); i++) {
        DType regType = regTypes[i];
        DType targetType = targetTypes[i];
	
        X86Gp castedReg = registerCast(c, regs[i], regType, targetType, destructive);
	X86Gp resultingReg = Generate::typedRegister(c, targetType)[0];

	c.mov(resultingReg, castedReg);
	
        result.push_back(resultingReg);
    }
    
    return result;
}


namespace Generate {
    
    /**
     * Cast expression to type.
     * If destructive is true registers used by the casted value may be reused.
     */
    Regs cast(X86Compiler& c, Expr* expr, DType& type, bool destructive) {
	Regs e = expr->generate(c);

	// Reference -> value
	if (expr->type.ref && !type.ref)
	    e = dereference(c, e[0], expr->type);
	
	Regs result;
		
        if (type == expr->type) {
	    if (destructive) {
		result = e;
	    } else {
		result = Generate::typedRegister(c, type);
		
		for (int i = 0; i < result.size(); i++)
		    c.mov(result[i], e[i]);
	    }
	    
	} else if (type.isPrimitive())
            result = { primitiveCast(c, e, expr->type, type, destructive) };
        else if (type.isTuple())
            result = tupleCast(c, e, expr->type, type, destructive);
        else
            assert(false);


	if (!expr->type.ref && type.ref) { // value -> ref
	    X86Gp heapPtr = Generate::alloc(c, type.byteSize());
	    X86Mem heapMem = x86::ptr(heapPtr);
	    
	    std::vector<DType> flatType = flattenType(type);
	    int offset = 0;
	    for (int i = 0; i < flatType.size(); i++) {
		heapMem.setOffset(offset);
		c.mov(heapMem, result[i]);
		
		offset += flatType[i].byteSize();
	    }

	    return { heapPtr };
	}
	
        return result;
    }
}

