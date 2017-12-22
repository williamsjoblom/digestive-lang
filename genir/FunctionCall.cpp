#include "FunctionCall.h"

TACOp Generate::functionCall(TACFun* fun, FunctionCall* call) {
    // Assign parameters
    TACType indexType(TACKind::UNSIGNED, 4);
    for (unsigned int i = 0; i < call->arguments->size(); i++) {
	TACOp arg = call->arguments->at(i)->generate(fun);
	fun->add(TACC::setArg, fun->newImm(indexType, i), arg, TACOp());
    }

    FunctionDecl* decl = call->declaration;
    TACType returnType = decl->returnType;

    TACOp returnValue;
    if (returnType.byteSize > 0)
	returnValue = fun->newVar(returnType);
    else
	returnValue = TACOp();
	
    fun->add(TACC::call, TACOp(decl), TACOp(), returnValue);
    
    return returnValue;
}
