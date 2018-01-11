#include "Instr.h"

#include "TACCompiler.h"
#include "Op.h"
#include "ir/TAC.h"
#include "ir/TACOp.h"
#include "jit/JitContext.h"

/*
#define GENERATE(INSTR, ENV) void _emit_(INSTR)(TACCompiler* tc, X86Compiler& c,	\
				     std::vector<Label>& labels,	\
				     TACOp& _s0, TACOp& _s1, TACOp& _d)
*/
#define GENERATE(INSTR, ENV) void (INSTR)(InstrEnv& (ENV))

#define EMIT(INSTR, ENV) (INSTR)((ENV))


/**
 * Arithmetics
 */
GENERATE(add, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);    
    Operand d  = generateOperand(e, e._d);
    
    assert(d.isRegOrMem());

    e.c.emit(X86Inst::kIdMov, d, s0);
    e.c.emit(X86Inst::kIdAdd, d, s1);
}

GENERATE(sub, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);    
    Operand d  = generateOperand(e, e._d);
    
    assert(d.isRegOrMem());

    e.c.emit(X86Inst::kIdMov, d, s0);
    e.c.emit(X86Inst::kIdSub, d, s1);
}

GENERATE(mul, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);    
    Operand d  = generateOperand(e, e._d);

    assert(d.isRegOrMem());
    
    e.c.emit(X86Inst::kIdMov, d, s0);
    if (e._d.type.isSigned())
	e.c.emit(X86Inst::kIdImul, d, s1);
    else
	e.c.emit(X86Inst::kIdMul, d, s1);
}

GENERATE(div, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);    
    Operand d  = generateOperand(e, e._d);

    assert(d.isRegOrMem());

    Operand s1RegOrMem;
    if (s1.isImm()) {
	s1RegOrMem = e.c.newGpReg(e._s1.type.asmjitId());
	e.c.emit(X86Inst::kIdMov, s1RegOrMem, s1);
    } else {
	s1RegOrMem = s1;
    }
    
    e.c.emit(X86Inst::kIdMov, d, s0);
    X86Gp remainder = e.c.newInt32();
    e.c.emit(X86Inst::kIdMov, remainder, Imm(0));

    if (e._d.type.isSigned())
	e.c.emit(X86Inst::kIdIdiv, remainder, d, s1RegOrMem);
    else
	e.c.emit(X86Inst::kIdDiv, remainder, d, s1RegOrMem);
}

/**
 * Comparisons
 */
GENERATE(cmpEQ, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);    
    Operand d  = generateOperand(e, e._d);

    assert(d.isRegOrMem());
    assert(false);
    //e.c.emit(X86Inst::kIdCmp, s0, s1);
    //e.c.emit(X86Inst::kIdSete, d.);
}

/**
 * Branching.
 */
GENERATE(pushArg, e) {
    e.tc->pushedArgs.push_back(e._s0);
}

GENERATE(call, e) {
    Operand s0 = generateOperand(e, e._s0);
    
    // Create function signature from pushed args.
    FuncSignatureX signature;
    for (TACOp& op : e.tc->pushedArgs) {
	signature.addArg(op.type.asmjitId());
    }

    if (!e._d.ignore) {
	Operand d  = generateOperand(e, e._d);
	signature.setRet(e._d.type.asmjitId());
    }

    // Emit call.
    CCFuncCall* funcCall;
    if (e._s0.type.kind == TACKind::PTR && s0.isReg()) {
	const X86Gp addrReg = s0.as<X86Gp>();
	funcCall = e.c.call(addrReg, signature);
    } else if (e._s0.type.kind == TACKind::PTR && s0.isImm()) {
	const Imm addrImm = s0.as<Imm>();
	funcCall = e.c.call(addrImm, signature);
    } else if(e._s0.kind == TACOpKind::FUNCTION) {
	X86Gp handle = e.c.newGpd();
	e.c.emit(X86Inst::kIdMov, handle, imm_ptr(JitContext::handles + e._s0.data.functionId));
	funcCall = e.c.call(handle, signature);
    } else {
	assert(false);
    }

    // Add pushed arguments to call.
    for (int i = 0; i < e.tc->pushedArgs.size(); i++) {
	Operand arg = generateOperand(e, e.tc->pushedArgs[i]);
	if (arg.isImm())
	    funcCall->setArg(i, arg.as<Imm>());
	else if (arg.isReg())
	    funcCall->setArg(i, arg.as<X86Gp>());
    }

    // Clear arguments.
    e.tc->pushedArgs.clear();
}

GENERATE(ret, e) {
    Operand s0 = generateOperand(e, e._s0);
    
    X86Gp returnReg;
    if (s0.isImm()) {
	returnReg = e.c.newGpReg(e._s0.type.asmjitId());
	e.c.emit(X86Inst::kIdMov, returnReg, s0);
    } else if (s0.isMem()) {
	returnReg = e.c.newGpReg(e._s0.type.asmjitId());
	e.c.emit(X86Inst::kIdMov, returnReg, s0);
    } else if (s0.isReg()) {
	returnReg = s0.as<X86Gp>();
    } else {
	assert(false);
    }
    
    e.c.ret(returnReg);
}

GENERATE(jmp, e) {
    Operand s0 = generateOperand(e, e._s0);
    e.c.jmp(s0.as<Label>());
}

GENERATE(jmpZ, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);

    // Optimize immediate comparisons.
    if (e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s1.data.immValue == 0)
	    e.c.emit(X86Inst::kIdJmp, s0);
    } else {
	e.c.emit(X86Inst::kIdCmp, s1);
	e.c.emit(X86Inst::kIdJz, s0);
    }
}


void generateInstr(InstrEnv& e, TAC* instr) {    
    // TODO: turn this (at least soon to be) massive switch
    // into a function table.
    switch (instr->op) {
    case TACC::add: EMIT(add, e); break;
    case TACC::sub: EMIT(sub, e); break;
    case TACC::mul: EMIT(mul, e); break;
    case TACC::div: EMIT(div, e); break;
    
    case TACC::call: EMIT(call, e); break;
    case TACC::pushArg: EMIT(pushArg, e); break;
    case TACC::ret: EMIT(ret, e); break;

    case TACC::jmp: EMIT(jmp, e); break;
    case TACC::jmpZ: EMIT(jmpZ, e); break;
    default: assert(false); // Not implemented.
    }
}
