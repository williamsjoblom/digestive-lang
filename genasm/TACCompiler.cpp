#include "TACCompiler.h"

#include "Instr.h"
#include "Op.h"
#include "gen/ErrHandler.h"

TACCompiler::TACCompiler() {
    
}


ProgramType TACCompiler::compile(JitRuntime& runtime, TACProgram& program) {
    for (TACFun* fun : program.functions)
	compileFun(runtime, fun);

    compileFun(runtime, program.entry);

    return nullptr;
}


FuncSignatureX createSignature(TACFun* fun) {
    FuncSignatureX signature(CallConv::kIdHostCDecl);
    signature.setRet(fun->returnType.asmjitId());
    
    for (TACVar* param : fun->parameters) {
	signature.addArg(param->type.asmjitId());
    }
    
    return signature;
}


void TACCompiler::compileFun(JitRuntime& runtime, TACFun* fun) {
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
    
    for (int i = 0; i < fun->parameters.size(); i++) {
	X86Gp reg = generateVar(this, c, fun->parameters[i]);
	c.setArg(i, reg);
    }

    for (TAC* instr : fun->instr) {
	generateInstr(this, c, instr);
    }

    c.endFunc();
    c.finalize();
    
    std::cout << std::endl << fun->identifier << "(): " << std::endl << logger.getString() << std::endl;
}
