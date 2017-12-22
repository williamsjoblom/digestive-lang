#pragma once

#include "TACCompiler.h"

#define OP(o) (						\
	(o).isReg() ? (o).as<X86Gp> :			\
	(o).isImm() ? (o).as<Imm> :			\
	(o).as<Label>					\
	)

/**
 * Generate variable.
 */
X86Gp generateVar(TACCompiler* tc, X86Compiler& c, TACOp var);


/**
 * Generate operand.
 */
Operand generateOperand(TACCompiler* tc, X86Compiler& c, TACOp op);
