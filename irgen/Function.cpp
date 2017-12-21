
#include "Function.h"

void Generate::function(TACFun* fun, FunctionDecl* decl) {
    decl->body->generate(fun);

    TACType implicitReturnType(TACKind::SIGNED, 4);
    fun->add(TACC::ret, fun->newImm(implicitReturnType, 0), TACOp(), TACOp());

    decl->irId = fun->id;
}
