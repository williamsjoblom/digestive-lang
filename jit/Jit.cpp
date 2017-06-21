//
// Created by wax on 1/30/17.
//

#include "Jit.h"

#include <fstream>
#include <sstream>
#include <assert.h>
#include <execinfo.h>
#include <bits/signum.h>
#include <signal.h>

#include "gen/Gen.h"
#include "parse/Parse.h"
#include "lexer/Lexer.h"
#include "JitContext.h"

struct stack_frame {
    struct stack_frame* next;
    void* ret;
};

int get_call_stack(void** retaddrs, int max_size) {
    /* x86/gcc-specific: this tells gcc that the fp
       variable should be an alias to the %ebp register
       which keeps the frame pointer */
    register struct stack_frame* fp asm("ebp");
    /* the rest just walks through the linked list */
    struct stack_frame* frame = fp;
    retaddrs[0] = frame->ret;
    retaddrs[1] = frame->next->ret;
    retaddrs[2] = frame->next->next->ret;
    return 3;
    /*int i = 0;
    while(frame) {
        if(i < max_size) {
            retaddrs[i++] = frame->ret;
        }
        frame = frame->next;
    }
    return i;*/
}

void signal_handler(int signo) {
    std::cout << "Signal received; backtrace:" << std::endl;
    void* trace[32];
    int traceSize = get_call_stack(trace, 32);

    //int traceSize = backtrace(trace, 32);
    //char** symbols = backtrace_symbols(trace, traceSize);

    for (int i = 0; i < traceSize; i++) {
        std::cout << trace[i] << std::endl;
    }

    std::cout << std::endl;
}

Jit::Jit() {
    program = nullptr;
    signal(SIGTRAP, signal_handler);
}

Jit::~Jit() {
    runtime.release((void*) program);
    delete JitContext::root;
}

bool Jit::load(std::string path) {
    std::string source = readFile(path);

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);

    try {
        JitContext::root = Parse::unit(tokens);
        Scope fileScope;
        JitContext::root->analyze(&fileScope);


        allocateHandles(JitContext::root->functionCount());

        program = Generate::program(&runtime, JitContext::root);
        JitContext::dumpHandles();
    } catch (int i) {
        std::cout << "compilation error " << i << std::endl;
        return false;
    }

    return true;
}

bool Jit::reload(std::string path) {
    Scope* fileScope = new Scope();

    try {
        std::cout << "Reloading '" << path << "'"<< std::endl;
        std::string source = readFile(path);

        Lexer lexer;
        TokenQueue tokens = lexer.lex(source);

        Unit* newRoot = Parse::unit(tokens);
        newRoot->analyze(fileScope);

        int updated = 0;
        int added = 0;

        std::cout << "Parsed " << newRoot->functions.size() << " functions" << std::endl;

        // TODO: O(N^2), will be slow for large files later on! Also not very pretty!
        for (FunctionDecl* newFunction : newRoot->functions) {
            bool addFunction = true;

            for (int i = 0; i < JitContext::root->functions.size(); i++) {
                FunctionDecl* oldFunction = JitContext::root->functions[i];

                if (*newFunction == *oldFunction) {                        // Function not updated
                    newFunction->bHandleIndex = oldFunction->bHandleIndex;
                    addFunction = false;
                    break;

                } else if (newFunction->matchesSignature(*oldFunction)) {  // Function body updated.
                    newFunction->bHandleIndex = oldFunction->bHandleIndex;

                    void* ptr = Generate::function(&runtime, newFunction);
                    JitContext::setHandle(newFunction->bHandleIndex, ptr);

                    JitContext::root->functions[i] = newFunction;
                    runtime.release((void*) oldFunction);
                    updated++;
                    addFunction = false;
                    break;
                }
            }

            if (addFunction) {

                unsigned int handleCount = JitContext::handleCount;

                void** handles = new void*[handleCount + 1];

                for(int i = 0; i < handleCount; i++) {
                    handles[i] = JitContext::handles[i];
                }

                newFunction->bHandleIndex = JitContext::handleCount;
                void* ptr = Generate::function(&runtime, newFunction);
                unsigned int index = JitContext::addHandle(ptr);
                assert(newFunction->bHandleIndex == index);

                JitContext::root->functions.push_back(newFunction);

                added++;
            }
        }

        /*
        std::cout << "Previous function handles: ";
        for (FunctionDecl* oldFunction : root->functions) {

        }
        */
        std::cout << "New function handles: ";
        for (FunctionDecl* newFunction : newRoot->functions) {
            std::cout << "[" << newFunction->identifier
                      << " [" << newFunction->bHandleIndex << "]"
                      << "=" << JitContext::handles[newFunction->bHandleIndex]
                      << "] ";

        }

        std::cout << std::endl;

        /*
        if (root->functions.size() != this->root->functions.size()) return false;

        for (int i = 0; i < root->functions.size(); i++) {
            Stmt* function =  root->functions[i];
            Stmt* oldFunction = this->root->functions[i];

            if (*function != *oldFunction) return false;
        }
        */

        std::cout << "Added " << added << " function(s)" << std::endl;
        std::cout << "Updated " << updated << " function(s)" << std::endl;

    } catch (int i) {
        std::cout << "Runtime error " << i << std::endl;
        return false;
    }

    return true;
}

std::string Jit::readFile(std::string path) {
    std::ifstream t(path);
    std::stringstream buffer;

    if(!t.is_open()) {
        std::cout << "Failed to open file: " << path << std::endl;
        throw 1;
    }

    buffer << t.rdbuf();

    return buffer.str();
}

int Jit::run() {
    return program();
}

void Jit::allocateHandles(unsigned int functionCount) {
    unsigned int capacity = 128 + functionCount * 2;
    JitContext::allocateHandles(capacity);
}



