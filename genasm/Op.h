#pragma once

#include "TACCompiler.h"

#define OP(o) (						\
	(o).isReg() ? (o).as<X86Gp> :			\
	(o).isImm() ? (o).as<Imm> :			\
	(o).as<Label>					\
	)

/**
 * Forward declarations.
 */
class InstrEnv;


/**
 * Generate variable.
 */
X86Gp generateVar(InstrEnv& e, TACOp& var);


/**
 * Generate operand.
 */
Operand generateOperand(InstrEnv& e, TACOp& op);
