#include "PrettyPrint.hh"

#include <iostream>


void printIndent(size_t indent) {
    std::cout << std::string(INDENT_LENGTH * indent, ' ');
}
