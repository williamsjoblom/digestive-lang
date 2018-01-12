#ifndef DIG_TACENVOP_H
#define DIG_TACENVOP_H

#include <iostream>
#include <string>

#include "TACType.h"
#include "util/PrettyPrint.h"
#include "util/Colors.h"

class TAC;
class TACOp;


/**
 * Label.
 */
struct TACLabel {
    int id;
    TAC* tac;
    std::string name;

    void dump(int indent) {
	printIndent(indent);
	std::cout << UNDL(<< name <<) << ":" << std::endl; };
};


/**
 * Variable.
 */
struct TACVar {
    int id;
    int byteSize;
    TACType type;
    std::string name;
};

#endif
