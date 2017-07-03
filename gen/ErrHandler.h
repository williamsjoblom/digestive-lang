//
// Created by wax on 7/3/17.
//

#ifndef DIG_ERRHANDLER_H
#define DIG_ERRHANDLER_H


#include "Base.h"

class ErrHandler : public asmjit::ErrorHandler {
    virtual bool handleError(Error err, const char* message, CodeEmitter* origin);
};


#endif //DIG_ERRHANDLER_H
