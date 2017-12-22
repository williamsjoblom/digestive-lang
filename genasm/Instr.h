#pragma once

#include <asmjit/asmjit.h>

#include "TACCompiler.h"

using namespace asmjit;

/**
 * Forward declarations.
 */
class TAC;
class TACOp;


/**
 * Generate instruction.
 */
void generateInstr(TACCompiler* tc, X86Compiler& c, TAC* instr);


/**
 * Generate variable.
 */
X86Gp generateVar(X86Compiler& c, TACOp var);

