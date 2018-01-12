#pragma once

#include <vector>

#include "TACFun.h"

/**
 * Three address code program.
 */
class TACProgram {
public:
    
    /**
     * Functions.
     */
    std::vector<TACFun*> functions;

    /**
     * Entry point.
     */
    TACFun* entry;

    
    /**
     * Constructor.
     */
    TACProgram();

    
    /**
     * Destructor.
     */
    ~TACProgram();

    
    /**
     * Get TACFun from id.
     */
    TACFun* fun(int id);

    
    /**
     * Dump.
     */
    void dump();
};
