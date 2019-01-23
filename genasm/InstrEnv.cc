#include "InstrEnv.hh"

#include "ir/TACFun.hh"


InstrEnv::InstrEnv(TACCompiler* tc, X86Compiler& c, TACFun* fun) : tc(tc), c(c) {
    for (unsigned int i { 0 }; i < fun->labels.size(); i++) {
	labels.push_back(c.newLabel());
    }
}


void InstrEnv::setContext(TAC* instr) {
    _s0 = instr->s0;
    _s1 = instr->s1;
    _d  = instr->d;
}


Label InstrEnv::label(int id) {
    return labels[id];
}
