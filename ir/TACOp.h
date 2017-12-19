#ifndef DIG_TACOP_H
#define DIG_TACOP_H

#include <string>

#include "TACEnvOp.h"
#include "TACType.h"

class TACEnv;

/**
 * Three address operand kind.
 */
enum TACOpKind {
    VARIABLE,
    IMMEDIATE,
    LABEL,
};


union TACOpData {
    unsigned long immValue;
    int labelId;
    int varId;
};

/**
 * Three address operand.
 */
class TACOp {
public:
    /**
     * Kind.
     */
    TACOpKind kind;


    /**
     * Type.
     */
    TACType type;

    /**
     * Data.
     * Constains immedate value, label id etc. depending on kind.
     */
    TACOpData data;

    /**
     * Ignore.
     * True if this operand is unused.
     */
    bool ignore;

    
    /**
     * Default constructor.
     */
    TACOp(bool used=false);


    /**
     * Label constructor.
     */
    TACOp(TACLabel* label);


    /**
     * Variable constructor.
     */
    TACOp(TACVar* var);

    
    /**
     * To string.
     */
    std::string toS(TACEnv* env) const;
};

#endif
