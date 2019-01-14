#include "BinaryExpr.hh"

#include "ast/IntegerLiteral.hh"
#include "ast/VariableExpr.hh"


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
 * Returns actual memory offset from a tuple access.
 */
unsigned long tupleAccessOffset(BinaryExpr* expr) {
    std::vector<DType>* tupleType = expr->left->type.type.tuple;
    int index = tupleAccessIndex(expr);

    unsigned long offset = 0;
    for (int i = 0; i < index; i++) {
	TACType t = tupleType->at(i);
	offset += t.byteSize;
    }
    
    return offset;
}


/**
 * Generate tuple access.
 */
TACOp tupleAccess(TACFun* fun, BinaryExpr* expr) {
    std::vector<DType>* tupleType = expr->left->type.type.tuple;
    int index = tupleAccessIndex(expr);
    unsigned long offset = tupleAccessOffset(expr);
    
    TACOp tuplePtr = expr->left->generate(fun);
    tuplePtr.offset = offset;
    tuplePtr.type.ref = true;
    
    if (expr->type.ref) { // Nested tuple, calculate new ptr.
	TACType type(tupleType->at(index));
	TACOp indirectPtr = fun->newVar(type);
	indirectPtr.type.ref = false;
	
	fun->add(TACC::cast, tuplePtr, TACOp(), indirectPtr);
	return indirectPtr;
    } else {
	return tuplePtr;
    }
}


/**
 * Generate primitive assignment.
 */
TACOp primitiveAssignment(TACFun* fun, VariableExpr* variable, Expr* value) {
    TACOp var = variable->generate(fun);
    TACOp val = value->generate(fun);
    
    fun->add(TACC::cast, val, TACOp(), var);
    
    return value;
}


/**
 * Generate tuple member assignment.
 */
TACOp memberAssignment(TACFun* fun, BinaryExpr* tupleAccess, Expr* value) {
    TACOp tuplePtr = tupleAccess->left->generate(fun);
    
    TACOp val = value->generate(fun);
    TACOp offset = fun->newImm(tupleAccessOffset(tupleAccess));
    
    fun->add(TACC::tupTo, val, offset, tuplePtr);

    return value;
}


/**
 * Generate assignment.
 */
TACOp assignment(TACFun* fun, BinaryExpr* expr) {
    VariableExpr* variable = dynamic_cast<VariableExpr*>(expr->left);
    if (variable)
	return primitiveAssignment(fun, variable, expr->right);
    
    BinaryExpr* tupleAccess = dynamic_cast<BinaryExpr*>(expr->left);
    if (tupleAccess && tupleAccess->op->symbol == OperatorSymbol::DOT)
	return memberAssignment(fun, tupleAccess, expr->right);
    
    assert(false); // Should be caught during semantic analysis.
}



TACOp Generate::binaryExpr(TACFun* fun, BinaryExpr* expr) {
    if (expr->op->symbol == OperatorSymbol::ASSIGN) {
        return assignment(fun, expr);
    } else if (expr->left->type.isPrimitive() && expr->right->type.isPrimitive()) {
	return primitiveBinaryExpr(fun, expr);
    } else if (expr->left->type.isTuple() && expr->op->symbol == OperatorSymbol::DOT) {
	return tupleAccess(fun, expr);
    }
	
    // Not returning before this point should have resulted in a
    // semantic error during analysis.
    assert(false);
}


