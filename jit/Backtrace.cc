#include <sys/ucontext.h>
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>

#include "util/Colors.hh"
#include "Backtrace.hh"
#include "JitContext.hh"


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



