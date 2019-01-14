#pragma once

#include <asmjit/asmjit.h>

class ErrHandler : public asmjit::ErrorHandler {
    bool handleError(asmjit::Error err, const char* message, asmjit::CodeEmitter* origin);
};
