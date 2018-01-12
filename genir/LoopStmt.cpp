#include "LoopStmt.h"

#include "ir/TACFunOp.h"

void Generate::loopStmt(TACFun* fun, LoopStmt* stmt) {
    TACOp condition = stmt->condition->generate(fun);
    
    TACLabel* startLabel = fun->newLabel();
    TACLabel* endLabel = fun->newLabel();
    TACOp startLabelOp = startLabel;
    TACOp endLabelOp = endLabel;
    
    fun->bindLabel(startLabel);
    fun->add(TACC::jmpZ, endLabelOp, condition, TACOp());
    
    stmt->body->generate(fun);

    fun->add(TACC::jmp, startLabelOp, TACOp(), TACOp());
    fun->bindLabel(endLabel);
}
