#include "ErrHandler.hh"

#include <iostream>

bool ErrHandler::handleError(asmjit::Error err, const char *message, asmjit::CodeEmitter *origin) {
    std::cout << "Assembler error: " << message << std::endl;
    return false;
}
