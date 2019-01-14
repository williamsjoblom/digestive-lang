#pragma once

#include <iostream>
#include <string>

#include "TACType.hh"
#include "util/PrettyPrint.hh"
#include "util/Colors.hh"

/**
 * Forward declarations.
 */
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
