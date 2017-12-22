#pragma once

#include <map>
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
     * Variable id to register.
     */
    std::map<int, X86Gp> varToReg;

    
    /**
     * Constructor.
     */
    TACCompiler();

    
    /**
     * Compile.
     */
    ProgramType compile(JitRuntime& runtime, TACProgram& program);

    
private:
    /**
     * Compile function.
     */
    void compileFun(JitRuntime& runtime, TACFun* fun);
};
