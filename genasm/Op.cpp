#include "Op.h"

#include "InstrEnv.h"

/**
 * Generate immediate operand.
 */
Imm generateImm(InstrEnv& e, TACOp& imm) {
    if (imm.type.isSigned())
	return asmjit::imm(imm.data.immValue);
    else
	return asmjit::imm_u(imm.data.immValue);
}


/**
 * Generate variable operand.
 */
X86Gp generateVar(InstrEnv& e, TACOp& var) {
    int varId = var.data.varId;
    
    auto it = e.tc->varToReg.find(varId);
    if (it == e.tc->varToReg.end()) {
	X86Gp reg = e.c.newGpReg(var.type.asmjitId());
	e.tc->varToReg[varId] = reg;
	return reg;
    } else {
	return e.tc->varToReg[varId];
    }
}


/**
 * Generate label operand.
 */
Label generateLabel(InstrEnv& e, TACOp& label) {
    return e.label(label.data.labelId);
}


Operand generateOperand(InstrEnv& e, TACOp& op) {
    switch (op.kind) {
    case TACOpKind::VARIABLE:
	return generateVar(e, op);
    case TACOpKind::IMMEDIATE:
	return generateImm(e, op);
    case TACOpKind::LABEL:
	return generateLabel(e, op);
    default:
	assert(false); // Not implemented.
    }
}
