#include "TAC.hh"

#include <iostream>

#include "TACFun.hh"
#include "util/PrettyPrint.hh"
#include "util/Colors.hh"

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
    
    "pushArg",
    "call",
    "ret",
    "jmp",

    "jmpZ",
    "jmpNZ",
    "jmpN",

    "move",
    "cast",
    "eaddr",
    "salloc",
    "tupTo",
    "tupFrom",

    "trap"
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
void TAC::dump(TACFun* env, int indent) {
    if (label != nullptr)
	label->dump(indent);

    printIndent(indent);
    std::cout << KBLU << BOLD(<< tacCodeToS[(int) op] <<) << " "
	      << KGRN << s0.toS(env) << " "
	      << KGRN << s1.toS(env) << " "
	      << KRED << d.toS(env) << RST;
}
