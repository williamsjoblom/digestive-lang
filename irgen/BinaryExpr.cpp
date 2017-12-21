#include "BinaryExpr.h"
#include "ast/IntegerLiteral.h"
#include "ast/VariableExpr.h"
#include "gen/CallConv.h"


/*
 * Generate primitive primitive binary expression.
 */
TACOp primitiveBinaryExpr(TACFun* env, BinaryExpr* expr) {
    DType& type = expr->type;
    assert(!type.isNilType());

    TACOp s0 = expr->left->generate(env);
    TACOp s1 = expr->right->generate(env);
    
    TACType dT = TACType(type);
    TACOp d = env->newVar(dT);

    switch(expr->op->symbol) {
    case OperatorSymbol::PLUS:
	env->add(TACC::add, s0, s1, d);
	break;
    case OperatorSymbol::MINUS:
	env->add(TACC::sub, s0, s1, d);
	break;
    case OperatorSymbol::MUL:
	env->add(TACC::mul, s0, s1, d);
	break;
    case OperatorSymbol::DIV:
	env->add(TACC::div, s0, s1, d);
	break;
    case OperatorSymbol::EQ:
	env->add(TACC::cmpEQ, s0, s1, d);
	break;
    case OperatorSymbol::NOTEQ:
	env->add(TACC::cmpNE, s0, s1, d);
	break;
    case OperatorSymbol::LESSEQ:
	env->add(TACC::cmpLE, s0, s1, d);
	break;
    case OperatorSymbol::GREATEREQ:
	env->add(TACC::cmpGE, s0, s1, d);
	break;
    case OperatorSymbol::LESS:
	env->add(TACC::cmpL, s0, s1, d);
	break;
    case OperatorSymbol::GREATER:
	env->add(TACC::cmpG, s0, s1, d);
	break;
    default:
	assert(false);
    }

    return d;
}


// Regs assignment(X86Compiler& c, BinaryExpr* expr) {
//     VariableExpr* variable = dynamic_cast<VariableExpr*>(expr->left);
//     assert(variable != nullptr); // Should be caught during semantic analysis.

//     Regs varRegs = variable->generate(c);
//     Regs valueRegs = Generate::cast(c, expr->right, variable->type);

//     assert(varRegs.size() <= valueRegs.size()); // Only shortening casts of tuples allowed.

//     for (int i = 0; i < varRegs.size(); i++) {
// 	c.mov(varRegs[i], valueRegs[i]);
//     }
    
//     return varRegs;
// }


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
TACOp valueTupleAccessExpr(TACFun* env, BinaryExpr* expr) {
    TACType indexType = TACType(TACKind::UNSIGNED, 4);
    int index = tupleFlatAccessIndex(expr);
    
    TACOp tuple = expr->left->generate(env);
    TACOp indexOperand = env->newImm(indexType, index);
            
    if (expr->type.isTuple()) {
        assert(false);
    } else if(expr->type.isPrimitive()) {
	assert(false);
    } else {
	assert(false);
    }
}

// Regs refTupleAccessExpr(X86Compiler& c, BinaryExpr* expr) {
//     // Calculate actual memory offset.
//     int index = tupleAccessIndex(expr);
    
//     std::vector<DType> flatType = flattenType(expr->left->type);
//     int offset = 0;
//     for (int i = 0; i < index; i++) {
// 	offset += flatType[i].byteSize();
//     }

//     Regs leftPtr = expr->left->generate(c);
//     assert(leftPtr.size() == 1);

//     X86Mem source = x86::ptr(leftPtr[0], offset);
//     Regs sink = Generate::typedRegister(c, expr->type);
    
//     if (expr->type.isTuple()) {
// 	int size = neededRegisterCount(expr->type);
	
// 	for (int i = 0; i < size; i++) {
// 	    source.setOffset(offset);
// 	    c.mov(sink[i], source);

// 	    offset += flatType[index + i].byteSize();
// 	}
//     } else if (expr->type.isPrimitive()) {
// 	c.mov(sink[0], source);
//     } else {
// 	assert(false);
//     }

//     return sink;
// }


// /*
//  * Generate tuple access expression: (a, b, c).2
//  *
//  * OPTIMIZATION POSSIBILITY: 
//  * Currently the accessed values are being moved to new registers.
//  * If the sink of the indexed tuple is constant or if the indexed tuple member
//  * is being assinged the move to new register should not occur.
//  *
//  * Currently all registers of the accessed tuple is generated.
//  * Is it viable to only generate the needed register(s)?
//  */
// Regs tupleAccessExpr(X86Compiler& c, BinaryExpr* expr) {
//     assert(expr->left->type.isTuple());
//     assert(expr->op->symbol == OperatorSymbol::DOT);


//     if (expr->left->type.ref) {
// 	return refTupleAccessExpr(c, expr);
//     } else {
// 	return valueTupleAccessExpr(c, expr);
//     }
// }

namespace Generate {
    TACOp binaryExpr(TACFun* env, BinaryExpr* expr) {
	if (expr->op->symbol == OperatorSymbol::ASSIGN) {
	    assert(false);
	    //return assignment(c, expr);
	} else if (expr->left->type.isPrimitive() && expr->right->type.isPrimitive()) {
	    return primitiveBinaryExpr(env, expr);
	} else if (expr->left->type.isTuple() && expr->op->symbol == OperatorSymbol::DOT) {
	    //return tupleAccessExpr(c, expr);
	}
	
	// Not returning before this point should have resulted in a semantic error during analysis of the binary expr.
	assert(false);
    }

}

