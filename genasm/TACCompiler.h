#pragma once

#include <map>
#include <vector>
#include <asmjit/asmjit.h>

#include "ir/TACProgram.h"

using namespace asmjit;

/**
 * Program type ( () -> int ).
 */
typedef int (*ProgramType)(void);

/**
 * Three address code compiler.
 */
class TACCompiler {
public:    
    /**
     * Pushed arguments.
     */
    std::vector<TACOp> pushedArgs;

    
    /**
     * Constructor.
     */
    TACCompiler();

    
    /**
     * Compile.
     */
    ProgramType compile(JitRuntime& runtime, TACProgram& program);

    /**
     * Compile function.
     */
    void* compileFun(JitRuntime& runtime, TACFun* fun);
};
