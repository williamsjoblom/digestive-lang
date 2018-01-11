#include "TACCompiler.h"

#include "Instr.h"
#include "Op.h"
#include "gen/ErrHandler.h"
#include "jit/JitContext.h"

TACCompiler::TACCompiler() {
    
}


ProgramType TACCompiler::compile(JitRuntime& runtime, TACProgram& program) {
    JitContext::allocateHandles(program.functions.size() + 1);
    
    void* entryPtr = compileFun(runtime, program.entry);

    std::vector<void*> funPtrs;
    for (TACFun* fun : program.functions) {
	void* funPtr = compileFun(runtime, fun);
	funPtrs.push_back(funPtr);
    }
    
    JitContext::addHandle(entryPtr);
    for (void* funPtr : funPtrs)
	JitContext::addHandle(funPtr);
    
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
 * Bind label present at the current instruction.
 */
void bindLabelAtPoint(TACFun* fun, InstrEnv& e, TAC* instr) {
    if (instr->label != nullptr)
	e.c.bind(e.label(instr->label->id));
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

    std::cout << "label count: " << e.labels.size() << std::endl;

    for (int i = 0; i < fun->parameters.size(); i++) {
	TACOp param = fun->parameters[i];
	X86Gp reg = generateVar(e, param);
	c.setArg(i, reg);
    }

    for (TAC* instr : fun->instr) {
	bindLabelAtPoint(fun, e, instr);
	    
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
