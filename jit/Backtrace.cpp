//
// Created by wax on 6/26/17.
//

#include <sys/ucontext.h>
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <util/Colors.h>
#include "Backtrace.h"
#include "JitContext.h"


void Backtrace::ssd() {
    void* framePtr = __builtin_frame_address(1);
    ssd(nullptr, framePtr);
}

void Backtrace::ssd(void* pc, void* framePtr) {

    if(pc) {
        StackFrame s; s.returnAddr = pc;
        dumpStackFrame(s);
    }

    while (framePtr) {
        StackFrame f = *(StackFrame*) framePtr;

        if(!dumpStackFrame(f));

        framePtr = f.next;
    }
}

bool Backtrace::dumpStackFrame(StackFrame& f) {
    std::string symbol = JitContext::root->symbolFromPtr(f.returnAddr);
    if (symbol.empty()) return false;

    std::cout << KWHT << "[" << f.returnAddr <<  "]" << KBLU << "  main" << RST << "::" << KBLU << symbol << RST << "(..)" << std::endl;

    return true;
}



