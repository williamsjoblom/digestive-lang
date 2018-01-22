#include "BuiltIn.h"

#include <string>

#include "ir/TACFun.h"
#include "ast/PlnStmt.h"
#include "jit/BuiltIn.h"


/**
 * Emit tuple pln.
 */
void emitTuplePln(TACFun* fun, PlnStmt* stmt) {
    TACOp ptr = fun->newImm<void*>((void*) BuiltIn::plnTuple);
    TACOp arg = stmt->expression->generate(fun);
    TACOp typePtr = fun->newImm<void*>(&stmt->expression->type);
    
    fun->add(TACC::pushArg, typePtr, TACOp(), TACOp());
    fun->add(TACC::pushArg, arg, TACOp(), TACOp());
    fun->add(TACC::call, ptr, TACOp(), TACOp());
}


/**
 * Emit primitive pln.
 */
void emitPrimitivePln(TACFun* fun, PlnStmt* stmt) {
    void* ptr;
    
    DPrimitiveKind pk = stmt->expression->type.type.primitive;
    int sz = stmt->expression->type.sz;
    if (pk == DPrimitiveKind::INTEGER && sz == 2)
	ptr = (void*) BuiltIn::plnPrimitive<short>;
    else if (pk == DPrimitiveKind::INTEGER && sz == 4)
	ptr = (void*) BuiltIn::plnPrimitive<int>;
    else if (pk == DPrimitiveKind::INTEGER && sz == 8)
	ptr = (void*) BuiltIn::plnPrimitive<long>;
    else if (pk == DPrimitiveKind::NATURAL && sz == 1)
	ptr = (void*) BuiltIn::plnPrimitive<char>;
    else if (pk == DPrimitiveKind::NATURAL && sz == 2)
	ptr = (void*) BuiltIn::plnPrimitive<unsigned short>;
    else if (pk == DPrimitiveKind::NATURAL && sz == 4)
	ptr = (void*) BuiltIn::plnPrimitive<unsigned int>;
    else if (pk == DPrimitiveKind::NATURAL && sz == 8)
	ptr = (void*) BuiltIn::plnPrimitive<unsigned long>;
    else
	assert(false);

    TACOp funPtr = fun->newImm<void*>(ptr);

    Expr* astArg = stmt->expression;
    TACOp arg = astArg->generate(fun);
    
    if (arg.type.ref) {
	// Dereference argument before passing.
	TACType derefType = astArg->type;
	derefType.ref = false;
	    
	TACVar* derefArg = fun->newVar(derefType);
	fun->add(TACC::cast, arg, TACOp(), derefArg);
	fun->add(TACC::pushArg, derefArg, TACOp(), TACOp());
    } else {
	fun->add(TACC::pushArg, arg, TACOp(), TACOp());
    }
    
    fun->add(TACC::call, funPtr, TACOp(), TACOp());
}


void Generate::pln(TACFun* fun, PlnStmt* stmt) {
    if (stmt->expression->type.isPrimitive()) {
	emitPrimitivePln(fun, stmt);
    } else if (stmt->expression->type.isTuple()) {
	emitTuplePln(fun, stmt);
    }    
}

