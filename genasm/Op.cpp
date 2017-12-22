#include "Op.h"


/**
 * Generate immediate operand.
 * TODO: remove need for register move.
 */
Imm generateImm(X86Compiler& c, TACOp imm) {
    if (imm.type.isSigned())
	return asmjit::imm(imm.data.immValue);
    else
	return asmjit::imm_u(imm.data.immValue);
}


/**
 * Generate variable.
 */
X86Gp generateVar(TACCompiler* tc, X86Compiler& c, TACOp var) {
    int varId = var.data.varId;
    
    auto it = tc->varToReg.find(varId);
    if (it == tc->varToReg.end()) {
	X86Gp reg = c.newGpReg(var.type.asmjitId());
	tc->varToReg[varId] = reg;
	return reg;
    } else {
	return tc->varToReg[varId];
    }
}


Operand generateOperand(TACCompiler* tc, X86Compiler& c, TACOp op) {
    switch (op.kind) {
    case TACOpKind::VARIABLE:
        return generateVar(tc, c, op);
    case TACOpKind::IMMEDIATE:
	return generateImm(c, op);
    default:
	assert(false);
    }
}
