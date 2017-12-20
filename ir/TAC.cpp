#include "TAC.h"

#include <iostream>

#include "TACEnv.h"

const char* tacCodeToS[] = {    
    "add",
    "sub",
    "mul",
    "div",


    "cmpEQ",
    "cmpNE",
    "cmpG",
    "cmpL",
    "cmpGE",
    "cmpLE",
    

    "setArg",
    "call",
    "jmp",

    "jmpZ",
    "jmpNZ",
    "jmpN",

    "move",
    "moveCast"
};


/**
 * Constructor.
 */
TAC::TAC(TACC op, TACOp& s0, TACOp& s1, TACOp& d) {
    this->op = op;
    this->s0 = s0;
    this->s1 = s1;
    this->d = d;
    this->label = nullptr;
}



/**
 * Dump.
 */
void TAC::dump(TACEnv* env, int indent) {
    if (label != nullptr)
	label->dump();
    
    std::cout << tacCodeToS[(int) op] << " " << s0.toS(env) << " "
	      << s1.toS(env) << " → " << d.toS(env);
}
