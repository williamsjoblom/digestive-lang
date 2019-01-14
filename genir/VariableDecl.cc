#include "VariableDecl.hh"

#include "ast/Expr.hh"

void Generate::variableDeclaration(TACFun* env, VariableDecl* decl) {
    TACType t = decl->type;
    TACVar* var = env->newVar(t);

    if (decl->value != nullptr) {
	TACOp value = decl->value->generate(env);
	env->add(TACC::cast, value, TACOp(), var);
    }

    decl->irVar = var;
}
