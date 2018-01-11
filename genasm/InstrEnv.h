#pragma once

#include <asmjit/asmjit.h>
#include "ir/TACOp.h"

/**
 * Forward declarations.
 */
class TACCompiler;
class TACFun;

using namespace asmjit;

/**
 * Instruction generation environment.
 */
class InstrEnv {
public:
    /**
     * TAC compiler.
     */
    TACCompiler* tc;
    
    /**
     * Compiler.
     */
    X86Compiler& c;

    /**
     * Labels
     */
    std::vector<Label> labels;
    
    /**
     * Source operand 0.
     */
    TACOp _s0;
    
    /**
     * Source operand 1.
     */
    TACOp _s1;
    
    /**
     * Destination operand.
     */
    TACOp _d;


    /**
     * Constructor.
     */
    InstrEnv(TACCompiler* tc, X86Compiler& c, TACFun* fun);

    
    /**
     * Set instruction context.
     */
    void setContext(TAC* instr);

    
    /**
     * Get label with id.
     */
    Label label(int id);
};
