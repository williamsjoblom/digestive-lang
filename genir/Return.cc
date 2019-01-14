#include "Return.hh"

void Generate::returnStmt(TACFun* fun, ReturnStmt* stmt) {
    TACOp returnValue = stmt->expression->generate(fun);
    fun->add(TACC::ret, returnValue, TACOp(), TACOp());
}
