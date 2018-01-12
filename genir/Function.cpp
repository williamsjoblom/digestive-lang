
#include "Function.h"

void Generate::function(TACFun* fun, FunctionDecl* decl) {
    decl->body->generate(fun);

    fun->add(TACC::ret, fun->newImm<int>(0), TACOp(), TACOp());

    decl->irId = fun->id;

    fun->dumpAssembly = decl->dumpAssembly;
    if (decl->dumpIr) fun->dump();
}
