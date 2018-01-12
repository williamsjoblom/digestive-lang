//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>
#include <assert.h>
#include <ast/type/PrimitiveType.h>

#include "Gen.h"
#include "CallConv.h"

using namespace asmjit;

/**
 * Generate primitive assignment.
 */
X86Gp primitiveAssign(X86Compiler& c, BinaryExpr* expr) {
}

/*
 * Generate primitive primitive binary expression.
 */
X86Gp primitiveBinaryExpr(X86Compiler &c, BinaryExpr* expr) {
    DType& type = expr->type;
    assert(!type.isNilType());
    
    Regs leftRegs = Generate::cast(c, expr->left, type);
    Regs rightRegs = Generate::cast(c, expr->right, type);
    assert(rightRegs.size() == 1 && leftRegs.size() == 1);

    X86Gp left = leftRegs[0];
    X86Gp right = rightRegs[0];

    assert(left.getSize() == right.getSize());

    Regs resultRegs = Generate::typedRegister(c, type);
    assert(resultRegs.size() == 1);
    X86Gp result = resultRegs[0];

    X86Gp remainder;
    switch(expr->op->symbol) {
    case OperatorSymbol::PLUS:
	c.mov(result, left);
	c.add(result, right);
	break;
    case OperatorSymbol::MINUS:
	c.mov(result, left);
	c.sub(result, right);
	break;
    case OperatorSymbol::MUL:
	c.mov(result, left);
	if (type.type.primitive == DPrimitiveKind::INTEGER)
	    c.imul(result, right);
	else
	    c.mul(result, right);
	break;
    case OperatorSymbol::DIV:
	c.mov(result, left);
	remainder = c.newInt32();
	c.mov(remainder, Imm(0));
	    
	if (type.type.primitive == DPrimitiveKind::INTEGER)	    
	    c.idiv(remainder, result, right);
	else
	    c.div(remainder, result, right);
	break;
	    
    case OperatorSymbol::EQ:
	c.cmp(left, right);
	c.sete(result.r8());
	c.movzx(result, result.r8());
	break;
    case OperatorSymbol::NOTEQ:
	c.cmp(left, right);
	c.setne(result.r8());
	c.movzx(result, result.r8());
	break;
    case OperatorSymbol::LESSEQ:
	c.cmp(left, right);
	c.setle(result.r8());
	c.movzx(result, result.r8());
	break;
    case OperatorSymbol::GREATEREQ:
	c.cmp(left, right);
	c.setge(result.r8());
	c.movzx(result, result.r8());
	break;
    case OperatorSymbol::LESS:
	c.cmp(left, right);
	c.setl(result.r8());
	c.movzx(result, result.r8());
	break;
    case OperatorSymbol::GREATER:
	c.cmp(left, right);
	c.setg(result.r8());
	c.movzx(result, result.r8());
	break;
    default:
	assert(false);
    }

    return result;
}


Regs assignment(X86Compiler& c, BinaryExpr* expr) {
    VariableExpr* variable = dynamic_cast<VariableExpr*>(expr->left);
    assert(variable != nullptr); // Should be caught during semantic analysis.

    Regs varRegs = variable->generate(c);
    Regs valueRegs = Generate::cast(c, expr->right, variable->type);

    assert(varRegs.size() <= valueRegs.size()); // Only shortening casts of tuples allowed.

    for (int i = 0; i < varRegs.size(); i++) {
	c.mov(varRegs[i], valueRegs[i]);
    }
    
    return varRegs;
}


/**
 * Returns index of tuple field.
 */
int tupleFlatAccessIndex(BinaryExpr* expr) {
    std::vector<DType> containedTypes = *expr->left->type.type.tuple;
    
    IntegerLiteral* literal = dynamic_cast<IntegerLiteral*>(expr->right);
    if (literal != nullptr) {
	int index = 0;
	for (int i = 0; i < literal->value; i++) {
	    DType& containedType = containedTypes[i];
	    index += neededRegisterCount(containedType);
	}

	return index;
    }

    VariableExpr* label = dynamic_cast<VariableExpr*>(expr->right);
    if (label != nullptr) {
	int index = 0;
	for (DType containedType : containedTypes) {
	    if (containedType.label == label->identifier)
		return index;
	    
	    index += neededRegisterCount(containedType);
	}
    }

    assert(false); // Should be caught during semantic analysis.
}

/**
 * Generate value tuple.
 */
Regs valueTupleAccessExpr(X86Compiler& c, BinaryExpr* expr) {
    int index = tupleFlatAccessIndex(expr);
    
    Regs leftRegs = expr->left->generate(c);

    if (expr->type.isTuple()) {
	Regs result;
	    
	int size = neededRegisterCount(expr->type);
	for (int i = 0; i < size; i++) {
	    X86Gp source = leftRegs[index + i];
	    X86Gp sink = c.newSimilarReg<X86Gp>(source, "sink");
	    c.mov(sink, source);
		
	    result.push_back(sink);
	}
	    
	return result;
    } else if(expr->type.isPrimitive()) {
	X86Gp source = leftRegs[index];
	X86Gp sink = c.newSimilarReg<X86Gp>(source, "sink");
	c.mov(sink, source);
	return { sink };
    } else {
	assert(false);
    }
}

Regs refTupleAccessExpr(X86Compiler& c, BinaryExpr* expr) {
    // Calculate actual memory offset.
    int index = tupleFlatAccessIndex(expr);
    
    std::vector<DType> flatType = flattenType(expr->left->type);
    int offset = 0;
    for (int i = 0; i < index; i++) {
	offset += flatType[i].byteSize();
    }

    Regs leftPtr = expr->left->generate(c);
    assert(leftPtr.size() == 1);

    X86Mem source = x86::ptr(leftPtr[0], offset);
    Regs sink = Generate::typedRegister(c, expr->type);
    
    if (expr->type.isTuple()) {
	int size = neededRegisterCount(expr->type);
	
	for (int i = 0; i < size; i++) {
	    source.setOffset(offset);
	    c.mov(sink[i], source);

	    offset += flatType[index + i].byteSize();
	}
    } else if (expr->type.isPrimitive()) {
	c.mov(sink[0], source);
    } else {
	assert(false);
    }

    return sink;
}


/*
 * Generate tuple access expression: (a, b, c).2
 *
 * OPTIMIZATION POSSIBILITY: 
 * Currently the accessed values are being moved to new registers.
 * If the sink of the indexed tuple is constant or if the indexed tuple member
 * is being assinged the move to new register should not occur.
 *
 * Currently all registers of the accessed tuple is generated.
 * Is it viable to only generate the needed register(s)?
 */
Regs tupleAccessExpr(X86Compiler& c, BinaryExpr* expr) {
    assert(expr->left->type.isTuple());
    assert(expr->op->symbol == OperatorSymbol::DOT);


    if (expr->left->type.ref) {
	return refTupleAccessExpr(c, expr);
    } else {
	return valueTupleAccessExpr(c, expr);
    }
}

namespace Generate {

    /*
     * Generate binary expression.
     */
    std::vector<X86Gp> expression(X86Compiler &c, BinaryExpr* expr) {
	if (expr->op->symbol == OperatorSymbol::ASSIGN) {
	    return assignment(c, expr);
	} else if (expr->left->type.isPrimitive() && expr->right->type.isPrimitive()) {
            return { primitiveBinaryExpr(c, expr) };
        } else if (expr->left->type.isTuple() && expr->op->symbol == OperatorSymbol::DOT) {
	    return tupleAccessExpr(c, expr);
	}
	
        // Not returning before this point should have resulted in a semantic error during analysis of the binary expr.
        assert(false);
    }

}

