#pragma once

#include <asmjit/asmjit.h>

#include "TACCompiler.h"
#include "InstrEnv.h"

using namespace asmjit;

/**
 * Forward declarations.
 */
class TAC;
class TACOp;

    
/**
 * Generate instruction.
 */
void generateInstr(InstrEnv& e, TAC* instr);


/**
 * Generate variable.
 */
X86Gp generateVar(X86Compiler& c, TACOp var);

