//
// Created by wax on 7/3/17.
//

#include <iostream>
#include "ErrHandler.h"

bool ErrHandler::handleError(Error err, const char *message, CodeEmitter *origin) {
    std::cout << "Error: " << message << std::endl;
    return false;
}
