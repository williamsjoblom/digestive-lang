#include "IfStmt.h"
#include "ir/TACFunOp.h"

void Generate::ifStmt(TACFun* env, IfStmt* stmt) {
    TACOp condition = stmt->condition->generate(env);
    TACLabel* endLabel = env->newLabel();

    // Jump to end label if condition is zero.
    env->add(TACC::jmpZ, endLabel, condition, TACOp());

    stmt->ifBlock->generate(env);
    
    env->bindLabel(endLabel);
    if (stmt->elseBlock != nullptr)
	stmt->elseBlock->generate(env);
}
