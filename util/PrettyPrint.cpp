//
// Created by wax on 12/14/16.
//

#include <iostream>

#include "PrettyPrint.h"

void printIndent(size_t indent) {
    std::cout << std::string(INDENT_LENGTH * indent, ' ');
}
