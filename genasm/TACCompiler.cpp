#include "TACCompiler.h"

#include "Instr.h"
#include "Op.h"
#include "gen/ErrHandler.h"

TACCompiler::TACCompiler() {
    
}


ProgramType TACCompiler::compile(JitRuntime& runtime, TACProgram& program) {
    std::vector<void*> funPtrs;
    for (TACFun* fun : program.functions) {	
	void* funPtr = compileFun(runtime, fun);
	funPtrs.push_back(funPtr);
    }

    void* entryPtr = compileFun(runtime, program.entry);
    return (ProgramType) entryPtr;
}


FuncSignatureX createSignature(TACFun* fun) {
    FuncSignatureX signature(CallConv::kIdHostCDecl);
    signature.setRet(fun->returnType.asmjitId());
    
    for (TACVar* param : fun->parameters) {
	signature.addArg(param->type.asmjitId());
    }
    
    return signature;
}


/**
 * Bind labels present at the current instruction.
 * TODO: Binding labels currently O(m*n). We can do better!
 */
void bindLabelsAtPoint(TACFun* fun, InstrEnv& e, TAC* instr) {
    for (TACLabel* tacLabel : fun->labels) {
	if (tacLabel->tac == instr)
	    e.c.bind(e.label(tacLabel->id));
    }
}


void* TACCompiler::compileFun(JitRuntime& runtime, TACFun* fun) {
    CodeHolder code;
    StringLogger logger;
    ErrHandler handler;
    code.init(runtime.getCodeInfo());
    code.setLogger(&logger);
    code.setErrorHandler(&handler);
    X86Compiler c(&code);
    
    FuncSignatureX signature = createSignature(fun);
    CCFunc* f = c.addFunc(signature);
    f->getFrameInfo().enablePreservedFP();

    InstrEnv e(this, c, fun);
	
    for (int i = 0; i < fun->parameters.size(); i++) {
	TACOp param = fun->parameters[i];
	X86Gp reg = generateVar(e, param);
	c.setArg(i, reg);
    }

    for (TAC* instr : fun->instr) {
	bindLabelsAtPoint(fun, e, instr);
	    
	e.setContext(instr);
	generateInstr(e, instr);
    }
	
    c.endFunc();
    c.finalize();

    void* funPtr;
    runtime.add(&funPtr, &code);
    
    std::cout << std::endl << fun->identifier << "(): " << std::endl << logger.getString() << std::endl;

    return funPtr;
}
