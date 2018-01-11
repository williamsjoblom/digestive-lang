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

/**
 * Emit x86 'cmp' instruction.
 * Immediate immediate comparisons should be optimized
 * out by the caller.
 */
void nativeCmp(InstrEnv& e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);

    // Comparisons where the first operand is an immediate value
    // not possible. Thus moving the immediate value to a register
    // is required.
    if (s0.isImm()) {
	X86Gp s0Reg = e.c.newGpReg(e._s0.type.asmjitId());
	e.c.emit(X86Inst::kIdMov, s0Reg, s0);
	s0 = s0Reg;
    }

    e.c.emit(X86Inst::kIdCmp, s0, s1);
}


GENERATE(cmpEQ, e) {    
    Operand d  = generateOperand(e, e._d);
    assert(d.isRegOrMem());
    
    // Optimize immediate comparison.
    if (e._s0.kind == TACOpKind::IMMEDIATE &&
	e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s0.data.immValue == e._s1.data.immValue)
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
	else
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
    }
    
    nativeCmp(e);
    e.c.emit(X86Inst::kIdSete, d);
}

GENERATE(cmpNE, e) {    
    Operand d  = generateOperand(e, e._d);
    assert(d.isRegOrMem());
    
    // Optimize immediate comparison.
    if (e._s0.kind == TACOpKind::IMMEDIATE &&
	e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s0.data.immValue != e._s1.data.immValue)
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
	else
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
    }
    
    nativeCmp(e);
    e.c.emit(X86Inst::kIdSetne, d);
}

GENERATE(cmpG, e) {    
    Operand d  = generateOperand(e, e._d);
    assert(d.isRegOrMem());
    
    // Optimize immediate comparison.
    if (e._s0.kind == TACOpKind::IMMEDIATE &&
	e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s0.data.immValue > e._s1.data.immValue)
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
	else
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
    }
    
    nativeCmp(e);
    e.c.emit(X86Inst::kIdSetg, d);
}

GENERATE(cmpL, e) {    
    Operand d  = generateOperand(e, e._d);
    assert(d.isRegOrMem());
    
    // Optimize immediate comparison.
    if (e._s0.kind == TACOpKind::IMMEDIATE &&
	e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s0.data.immValue < e._s1.data.immValue)
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
	else
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
    }
    
    nativeCmp(e);
    e.c.emit(X86Inst::kIdSetl, d);
}

GENERATE(cmpGE, e) {    
    Operand d  = generateOperand(e, e._d);
    assert(d.isRegOrMem());
    
    // Optimize immediate comparison.
    if (e._s0.kind == TACOpKind::IMMEDIATE &&
	e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s0.data.immValue >= e._s1.data.immValue)
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
	else
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
    }
    
    nativeCmp(e);
    e.c.emit(X86Inst::kIdSetge, d);
}

GENERATE(cmpLE, e) {    
    Operand d  = generateOperand(e, e._d);
    assert(d.isRegOrMem());
    
    // Optimize immediate comparison.
    if (e._s0.kind == TACOpKind::IMMEDIATE &&
	e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s0.data.immValue <= e._s1.data.immValue)
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
	else
	    e.c.emit(X86Inst::kIdMov, d, Imm(1));
    }
    
    nativeCmp(e);
    e.c.emit(X86Inst::kIdSetle, d);
}


/**
 * Branching.
 */
GENERATE(pushArg, e) {
    e.tc->pushedArgs.push_back(e._s0);
}

GENERATE(call, e) {
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
    if (e._s0.type.kind == TACKind::PTR) {
	Operand s0 = generateOperand(e, e._s0);
	
	if (s0.isReg()) {
	    const X86Gp addrReg = s0.as<X86Gp>();
	    funcCall = e.c.call(addrReg, signature);
	} else if (s0.isImm()) {
	    const Imm addrImm = s0.as<Imm>();
	    funcCall = e.c.call(addrImm, signature);
	}
    } else if(e._s0.kind == TACOpKind::FUNCTION) {
	X86Gp handle = e.c.newGpd();
	funcCall = e.c.call(x86::ptr((uint64_t)(JitContext::handles + e._s0.data.functionId)), signature);
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
	else
	    assert(false); // Not implemented.
    }

    // Set return.
    if (!e._d.ignore) {
	Operand ret = generateOperand(e, e._d);
	assert(ret.isReg());
	funcCall->setRet(0, ret.as<Reg>());
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
    e.c.emit(X86Inst::kIdJmp, s0);
}

GENERATE(jmpZ, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);

    // Optimize immediate comparisons.
    if (e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s1.data.immValue == 0)
	    e.c.emit(X86Inst::kIdJmp, s0);
    } else {
	e.c.emit(X86Inst::kIdTest, s1, s1);
	e.c.emit(X86Inst::kIdJz, s0);
    }
}

GENERATE(jmpNZ, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand s1 = generateOperand(e, e._s1);

    // Optimize immediate comparisons.
    if (e._s1.kind == TACOpKind::IMMEDIATE) {
	if (e._s1.data.immValue != 0)
	    e.c.emit(X86Inst::kIdJmp, s0);
    } else {
	e.c.emit(X86Inst::kIdTest, s1, s1);
	e.c.emit(X86Inst::kIdJnz, s0);
    }
}

GENERATE(cast, e) {
    Operand s0 = generateOperand(e, e._s0);
    Operand d = generateOperand(e, e._d);

    // NOTES:
    // Just a regular 'mov' as of now.
    // Should the IR 'move' instruction have the same behaviour?
    e.c.emit(X86Inst::kIdMov, d, s0);
}


void generateInstr(InstrEnv& e, TAC* instr) {    
    // TODO: turn this (at least soon to be) massive switch
    // into a function table.
    switch (instr->op) {
    case TACC::add: EMIT(add, e); break;
    case TACC::sub: EMIT(sub, e); break;
    case TACC::mul: EMIT(mul, e); break;
    case TACC::div: EMIT(div, e); break;
    
    case TACC::cmpEQ: EMIT(cmpEQ, e); break;
    case TACC::cmpNE: EMIT(cmpNE, e); break;
    case TACC::cmpG:  EMIT(cmpG, e); break;
    case TACC::cmpL:  EMIT(cmpL, e); break;
    case TACC::cmpGE: EMIT(cmpGE, e); break;
    case TACC::cmpLE: EMIT(cmpLE, e); break;
    
    case TACC::call:    EMIT(call, e); break;
    case TACC::pushArg: EMIT(pushArg, e); break;
    case TACC::ret:     EMIT(ret, e); break;

    case TACC::jmp:   EMIT(jmp, e); break;
    case TACC::jmpZ:  EMIT(jmpZ, e); break;	
    case TACC::jmpNZ: EMIT(jmpNZ, e); break;

    case TACC::cast: EMIT(cast, e); break;
	
    default: assert(false); // Not implemented.
    }
}
