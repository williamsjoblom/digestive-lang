#ifndef DIG_TACCODE_H
#define DIG_TACCODE_H

/**
 * Three address opcode.
 */
enum class TACC {
    /**
     * Arithmetic
     */
    add,
    sub, 
    mul, 
    div,

    /**
     * Comparison.
     */
    cmpEQ,
    cmpNE,
    cmpG,
    cmpL,
    cmpLE,
    cmpGE,
    
    /**
     * Branch
     */
    setArg,
    call,
    jmp,

    /* Conditional branch */
    jmpZ,
    jmpNZ,
    jmpN,
    
    /**
     * Move
     */
    move,
};

#endif
