
//#include "ast/VariableExpr.h"
#include "Expr.h"
#include "ast/VariableDecl.h"


TACOp Generate::integerLiteral(TACEnv& env, IntegerLiteral* e) {
    TACType t = TACType(TACKind::SIGNED, 4);
    return env.newImm(t, (unsigned long) e->value);
}


TACOp Generate::variable(TACEnv& env, VariableExpr* e) {
    assert(e->declaration->irVar != nullptr);
    return e->declaration->irVar;
}
