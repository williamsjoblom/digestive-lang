#include "TACCompiler.h"

#include "Instr.h"
#include "Op.h"
#include "gen/ErrHandler.h"
#include "jit/JitContext.h"
#include "util/Colors.h"

TACCompiler::TACCompiler() { }


ProgramType TACCompiler::compile(JitRuntime& runtime, TACProgram& program) {
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


/**
 * Create asmjit function signature from TACFun.
 */
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
    if (instr->label != nullptr) {
	TACLabel* tacLabel = instr->label;
	Label asmLabel = e.label(tacLabel->id);
	e.c.bind(asmLabel);
    }
}


void* TACCompiler::compileFun(JitRuntime& runtime, TACFun* fun) {
    CodeHolder code;
    ErrHandler handler;
   
    StringLogger logger;
    logger.setIndentation("    ");
    logger.addOptions(StringLogger::kOptionHexImmediate);
    logger.addOptions(StringLogger::kOptionImmExtended);
    
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
	bindLabelAtPoint(fun, e, instr);
	    
	e.setContext(instr);
	generateInstr(e, instr);
    }
    
    c.endFunc();
    c.finalize();

    void* funPtr;
    runtime.add(&funPtr, &code);

    if (fun->dumpAssembly) {
	std::cout << UNDL(<< BOLD(<< fun->identifier <<) <<)
		  << ":" << std::endl;
	std::cout << logger.getString();
    }
    return funPtr;
}
