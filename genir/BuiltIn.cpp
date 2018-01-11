#include "BuiltIn.h"

#include "ir/TACFun.h"
#include "ast/PlnStmt.h"
#include "jit/BuiltIn.h"


void Generate::pln(TACFun* fun, PlnStmt* stmt) {
    void* ptr;
    
    DPrimitiveKind pk = stmt->expression->type.type.primitive;
    int sz = stmt->expression->type.sz;
    if (pk == DPrimitiveKind::INTEGER && sz == 2)
	ptr = (void*) BuiltIn::pln<short>;
    else if (pk == DPrimitiveKind::INTEGER && sz == 4)
	ptr = (void*) BuiltIn::pln<int>;
    else if (pk == DPrimitiveKind::INTEGER && sz == 8)
	ptr = (void*) BuiltIn::pln<long>;
    else if (pk == DPrimitiveKind::NATURAL && sz == 1)
	ptr = (void*) BuiltIn::pln<char>;
    else if (pk == DPrimitiveKind::NATURAL && sz == 2)
	ptr = (void*) BuiltIn::pln<unsigned short>;
    else if (pk == DPrimitiveKind::NATURAL && sz == 4)
	ptr = (void*) BuiltIn::pln<unsigned int>;
    else if (pk == DPrimitiveKind::NATURAL && sz == 8)
	ptr = (void*) BuiltIn::pln<unsigned long>;
    else
	assert(false);

    TACOp ptrOp = fun->newImm<void*>(ptr);
    TACOp arg = stmt->expression->generate(fun);

    fun->add(TACC::pushArg, arg, TACOp(), TACOp());
    fun->add(TACC::call, ptrOp, TACOp(), TACOp());
}
