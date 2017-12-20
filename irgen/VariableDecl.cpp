#include "VariableDecl.h"

void Generate::variableDeclaration(TACEnv& env, VariableDecl* decl) {
    TACType t = decl->type;
    TACVar* var = env.newVar(t);

    if (decl->value != nullptr) {
	TACOp value = decl->value->generate(env);
	env.add(TACC::moveCast, value, TACOp(), var);
    }

    decl->irVar = var;
}
