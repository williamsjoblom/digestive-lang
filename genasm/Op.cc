#include "Op.hh"

#include "InstrEnv.hh"

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
    
    auto it = e.varToReg.find(varId);
    if (it == e.varToReg.end()) {
	X86Gp reg = e.c.newGpReg(var.type.asmjitId());
	e.varToReg[varId] = reg;
	return reg;
    } else {
	return e.varToReg[varId];
    }
}


/**
 * Generate label operand.
 */
Label generateLabel(InstrEnv& e, TACOp& label) {
    return e.label(label.data.labelId);
}


/**
 * Generate operand without offset.
 */
Operand generateValue(InstrEnv& e, TACOp& op) {
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


/**
 * Generate pointer dereference.
 */
X86Mem generateDereference(InstrEnv& e, TACOp& op) {
    if (op.kind == TACOpKind::IMMEDIATE) {
	return x86::ptr(op.data.immValue + op.offset);
    } else if (op.kind == TACOpKind::VARIABLE) {
	X86Gp base = generateValue(e, op).as<X86Gp>();
	return x86::ptr(base, op.offset);
    } else if (op.kind == TACOpKind::LABEL) {
	Label base = generateValue(e, op).as<Label>();
	return x86::ptr(base, op.offset);
    } else {
	assert(false);
    }
}


Operand generateOperand(InstrEnv& e, TACOp& op, bool tryDereference) {
    if (tryDereference && op.type.ref)
	return generateDereference(e, op);
    else
	return generateValue(e, op);
}

