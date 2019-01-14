#include "Expr.hh"

#include "ir/TACFun.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/VariableExpr.hh"
#include "ast/VariableDecl.hh"
#include "ast/TupleExpr.hh"
#include "ir/TACTypes.hh"


TACOp Generate::integerLiteral(TACFun* fun, IntegerLiteral* e) {
    TACType t = TACType(TACKind::SIGNED, 4);
    return fun->newImm(t, (unsigned long) e->value);
}


TACOp Generate::variable(TACFun* fun, VariableExpr* e) {
    assert(e->declaration->irVar != nullptr);
    return e->declaration->irVar;
}


TACOp Generate::tuple(TACFun* fun, TupleExpr* e) {
    TACOp byteSize = fun->newImm<int>(e->type.byteSize());
    TACOp tuplePtr = fun->newVar(TACTYPE_TUPLE);
    fun->add(TACC::salloc, byteSize, TACOp(), tuplePtr);
    
    int offset = 0;
    for (Expr* expr : e->values) {
	TACOp exprOp = expr->generate(fun);
	fun->add(TACC::tupTo, exprOp, fun->newImm<int>(offset), tuplePtr);
	offset += exprOp.type.byteSize;
    }
    
    return tuplePtr;
}
