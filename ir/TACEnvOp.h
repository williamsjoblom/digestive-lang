#ifndef DIG_TACENVOP_H
#define DIG_TACENVOP_H

#include <iostream>
#include <string>

#include "TACType.h"

class TAC;
class TACOp;


/**
 * Label.
 */
struct TACLabel {
    int id;
    TAC* tac;
    std::string name;

    void dump() { std::cout << name << ":" << std::endl; };
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
