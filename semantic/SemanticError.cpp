//
// Created by wax on 12/16/16.
//

#include <iostream>
#include "SemanticError.h"


void semanticError(std::string message) {
    std::cout << message << std::endl;
    throw 1;
}