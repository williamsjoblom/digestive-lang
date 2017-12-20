#ifndef DIG_TAC_H
#define DIG_TAC_H

#include "TACOp.h"
#include "TACCode.h"
#include "TACEnv.h"


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
    void dump(TACEnv* env, int indent=0);
};

#endif
