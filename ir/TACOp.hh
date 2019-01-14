#pragma once

#include <string>

#include "TACFunOp.hh"
#include "TACType.hh"
#include "ast/FunctionDecl.hh"

class TACFun;

/**
 * Three address operand kind.
 */
enum TACOpKind {
    IMMEDIATE,
    FUNCTION,
    VARIABLE,
    LABEL,
};


union TACOpData {
    unsigned long immValue;
    int functionId;
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
     * True if this operand is unused. Automatically set when using
     * default constructor.
     */
    bool ignore;

    /**
     * Memory offset.
     */
    long offset;

    
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
     * Function constructor.
     */
    TACOp(FunctionDecl* fun);

    inline bool isImmediate() { return kind == TACOpKind::IMMEDIATE; }
    inline bool isFunction() { return kind == TACOpKind::FUNCTION; }
    inline bool isVariable() { return kind == TACOpKind::VARIABLE; }
    inline bool isLabel() { return kind == TACOpKind::LABEL; }

    
    /**
     * To string.
     */
    std::string toS(TACFun* fun) const;
};
