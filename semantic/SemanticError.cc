#include "SemanticError.hh"

#include <iostream>


void semanticError(std::string message) {
    std::cout << message << std::endl;
    throw 1;
}
