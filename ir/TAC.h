#pragma once

#include "TACOp.h"
#include "TACCode.h"
#include "TACFun.h"


/**
 * Three address code.
 */
class TAC {
public:
    /**
     * Constructor.
     */
    TAC(TACC op, TACOp& s0, TACOp& s1, TACOp& d);
    
    /**
     * Opcode.
     */
    TACC op;
    
    /**
     * First source operand.
     */
    TACOp s0;
    
    /**
     * Second source operand.
     */
    TACOp s1;
    
    /**
     * Destination operand.
     */
    TACOp d;

    /**
     * Label at this instruction.
     */
    TACLabel* label;

    
    /**
     * Dump.
     */
    void dump(TACFun* env, int indent=0);
};
