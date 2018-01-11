#include "IfStmt.h"
#include "ir/TACFunOp.h"

void Generate::ifStmt(TACFun* env, IfStmt* stmt) {
    TACOp condition = stmt->condition->generate(env);
    TACLabel* elseLabel = env->newLabel();

    // Jump to end label if condition is zero.
    env->add(TACC::jmpZ, elseLabel, condition, TACOp());

    stmt->ifBlock->generate(env);

    if (stmt->elseBlock != nullptr) {
	TACLabel* endLabel = env->newLabel();
	env->add(TACC::jmp, endLabel, TACOp(), TACOp());
	env->bindLabel(elseLabel);
	
	stmt->elseBlock->generate(env);
	
	env->bindLabel(endLabel);
    } else {
	env->bindLabel(elseLabel);
    }
}
