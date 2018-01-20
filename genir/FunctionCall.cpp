#include "FunctionCall.h"

TACOp Generate::functionCall(TACFun* fun, FunctionCall* call) {
    // Assign parameters
    TACType indexType(TACKind::UNSIGNED, 4);
    for (unsigned int i = 0; i < call->arguments->size(); i++) {
	Expr* astArg = call->arguments->at(i);
	TACOp arg = astArg->generate(fun);

	if (arg.type.ref) {
	    // Dereference argument before passing.
	    TACType derefType = astArg->type;
	    derefType.ref = false;
	    
	    TACVar* derefArg = fun->newVar(derefType);
	    fun->add(TACC::cast, arg, TACOp(), derefArg);
	    fun->add(TACC::pushArg, derefArg, TACOp(), TACOp());
	} else {
	    fun->add(TACC::pushArg, arg, TACOp(), TACOp());
	}
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
