#include "BinaryExpr.h"
#include "ast/IntegerLiteral.h"
#include "ast/VariableExpr.h"
//#include "gen/CallConv.h"

/*
 * Generate primitive primitive binary expression.
 */
TACOp primitiveBinaryExpr(TACFun* fun, BinaryExpr* expr) {
    DType& type = expr->type;
    assert(!type.isNilType());

    TACOp s0 = expr->left->generate(fun);
    TACOp s1 = expr->right->generate(fun);
    
    TACType dT = TACType(type);
    TACOp d = fun->newVar(dT);

    switch(expr->op->symbol) {
    case OperatorSymbol::PLUS:
	fun->add(TACC::add, s0, s1, d);
	break;
    case OperatorSymbol::MINUS:
	fun->add(TACC::sub, s0, s1, d);
	break;
    case OperatorSymbol::MUL:
	fun->add(TACC::mul, s0, s1, d);
	break;
    case OperatorSymbol::DIV:
	fun->add(TACC::div, s0, s1, d);
	break;
    case OperatorSymbol::EQ:
	fun->add(TACC::cmpEQ, s0, s1, d);
	break;
    case OperatorSymbol::NOTEQ:
	fun->add(TACC::cmpNE, s0, s1, d);
	break;
    case OperatorSymbol::LESSEQ:
	fun->add(TACC::cmpLE, s0, s1, d);
	break;
    case OperatorSymbol::GREATEREQ:
	fun->add(TACC::cmpGE, s0, s1, d);
	break;
    case OperatorSymbol::LESS:
	fun->add(TACC::cmpL, s0, s1, d);
	break;
    case OperatorSymbol::GREATER:
	fun->add(TACC::cmpG, s0, s1, d);
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


TACOp assignment(TACFun* fun, BinaryExpr* expr) {
    VariableExpr* variable = dynamic_cast<VariableExpr*>(expr->left);
    assert(variable != nullptr); // Should be caught during semantic analysis.

    //Regs varRegs = variable->generate(c);
    //Regs valueRegs = Generate::cast(c, expr->right, variable->type);

    TACOp var = variable->generate(fun);
    TACOp value = expr->right->generate(fun);

    fun->add(TACC::cast, value, TACOp(), var);
    
    return value;
}

/**
 * Returns actual accessed index from a tuple access.
 */
int tupleAccessIndex(BinaryExpr* expr) {
    std::vector<DType> containedTypes = *expr->left->type.type.tuple;
    
    IntegerLiteral* literal = dynamic_cast<IntegerLiteral*>(expr->right);
    if (literal != nullptr) return literal->value;

    VariableExpr* label = dynamic_cast<VariableExpr*>(expr->right);
    if (label != nullptr) {
	for (int i = 0; i < containedTypes.size(); i++) {
	    if (containedTypes[i].label == label->identifier)
		return i;
	}
    }

    assert(false); // Should be caught during semantic analysis.
}


/**
 * Generate value tuple.
 */
TACOp valueTupleAccessExpr(TACFun* fun, BinaryExpr* expr) {
    std::vector<DType>* tupleType = expr->left->type.type.tuple;
    int index = tupleAccessIndex(expr);
    int offset = 0;
    
    for (int i = 0; i < index; i++) {
	TACType t = tupleType->at(i);
	offset += t.byteSize;
    }
        
    TACOp offsetOp = fun->newImm<int>(offset);

    TACType t = expr->type;
    TACVar* result = fun->newVar(t);

    TACOp tuplePtr = expr->left->generate(fun);
    fun->add(TACC::tupFrom, tuplePtr, offsetOp, result);

    return result;
}


/**
 * Generate tuple access expression.
 */
TACOp tupleAccessExpr(TACFun* fun, BinaryExpr* expr) {
    assert(expr->left->type.isTuple());
    assert(expr->op->symbol == OperatorSymbol::DOT);

    if (expr->left->type.ref) {
	assert(false);
    } else {
	return valueTupleAccessExpr(fun, expr);
    }
}


TACOp Generate::binaryExpr(TACFun* fun, BinaryExpr* expr) {
    if (expr->op->symbol == OperatorSymbol::ASSIGN) {
        return assignment(fun, expr);
    } else if (expr->left->type.isPrimitive() && expr->right->type.isPrimitive()) {
	return primitiveBinaryExpr(fun, expr);
    } else if (expr->left->type.isTuple() && expr->op->symbol == OperatorSymbol::DOT) {
	return tupleAccessExpr(fun, expr);
    }
	
    // Not returning before this point should have resulted in a
    // semantic error during analysis of the binary expr.
    assert(false);
}


