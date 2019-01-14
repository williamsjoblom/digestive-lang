//
// Created by wax on 6/26/17.
//

#ifndef DIG_STACK_H
#define DIG_STACK_H



namespace Backtrace {

    struct StackFrame {
        struct StackFrame* next;
        void* returnAddr;
    };

    /**
     * Short stack dump.
     * Pretty print compact stacktrace.
     */
    void ssd();
    void ssd(void* pc, void* framePtr);

    /**
     * Print stack frame.
     * Returns true if the supplied frame is valid.
     */
    bool dumpStackFrame(StackFrame& f);
};


#endif //DIG_STACK_H
