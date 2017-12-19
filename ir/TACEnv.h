#ifndef DIG_TACENV_H
#define DIG_TACENV_H

#include "TAC.h"

#include <vector>

#include "TACOp.h"
#include "TACEnvOp.h"

/**
 * Forward declaration.
 */
class TAC;

/**
 * Three address code environment.
 */
class TACEnv {
public:
    
    /**
     * Program.
     */
    std::vector<TAC*> program;
    
    /**
     * Labels.
     */
    std::vector<TACLabel*> labels;

    /**
     * Labels waiting to be bound to next instruction.
     * (When bind is called without an explicit instruction labels 
     *  end up here before being bound to the next instruction added.)
     */
    std::vector<TACLabel*> waitingLabels;

    /**
     * Variables.
     */
    std::vector<TACVar*> variables;

    
    /**
     * Constructor.
     */
    TACEnv();

    
    /**
     * Destructor.
     */
    ~TACEnv();

    
    /**
     * Add new three address code.
     */
    TAC* add(TACC opcode, TACOp s0, TACOp s1, TACOp d);

    
    /**
     * Create new label.
     */
    TACLabel* newLabel(std::string name="");


    /**
     * Get label with id.
     */
    TACLabel* label(int id) const;

    
    /**
     * Bind label to current position.
     */
    void bindLabel(TACLabel* label);

    
    /**
     * Bind label to instruction.
     */
    void bindLabel(TACLabel* label, TAC* tac);


    /**
     * Create new variable.
     */
    TACVar* newVar(TACType& type, std::string name="");

    
    /**
     * Get variable with id.
     */
    TACVar* var(int id);


    /**
     * New immediate.
     */
    TACOp newImm(TACType& type, unsigned long value);
    
    
    /**
     * Dump.
     */
    void dump();
    
private:
    /**
     * Bind and remove all labels in 'waitingLabels' to
     * given instruction.
     */
    inline void bindWaitingLabels(TAC* tac);
    
};

#endif
