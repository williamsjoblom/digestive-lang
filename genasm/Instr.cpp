#include "Instr.h"

#include "TACCompiler.h"
#include "Op.h"
#include "ir/TAC.h"
#include "ir/TACOp.h"

void add(TACCompiler* tc, X86Compiler& c, Operand s0, Operand s1, Operand d) {
    assert(d.isRegOrMem());

    if (s0.isReg() && d.isReg())
	c.mov(d.as<X86Gp>(), s0.as<X86Gp>());
    else if (s0.isImm() && d.isReg())
	c.mov(d.as<X86Gp>(), s0.as<Imm>());
    else
	assert(false);

    if (s1.isReg() && d.isReg())
	c.add(d.as<X86Gp>(), s1.as<X86Gp>());
    else if (s0.isImm() && d.isReg())
	c.add(d.as<X86Gp>(), s1.as<Imm>());
    else
	assert(false);
}

void generateInstr(TACCompiler* tc, X86Compiler& c, TAC* instr) {
    Operand s0 = generateOperand(tc, c, instr->s0);
    Operand s1 = generateOperand(tc, c, instr->s1);
    Operand d  = generateOperand(tc, c, instr->d);
    
    switch (instr->op) {
    case TACC::add:
	add(tc, c, s0, s1, d); break;
    default: assert(false); // Not implemented
    }
}
