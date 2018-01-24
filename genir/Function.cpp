
#include "Function.h"

void Generate::function(TACFun* fun, FunctionDecl* decl) {
    decl->irId = fun->id;
    
    decl->body->generate(fun);
    fun->add(TACC::ret, fun->newImm<int>(0), TACOp(), TACOp());

    fun->dumpAssembly = decl->dumpAssembly;
    if (decl->dumpIr) fun->dump();
}
