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
#include <bits/siginfo.h>
#include <sys/ucontext.h>
#include <util/Colors.h>

#include "globals.h"
#include "gen/Gen.h"
#include "parse/Parse.h"
#include "lexer/Lexer.h"
#include "JitContext.h"
#include "Backtrace.h"
#include "util/File.h"


struct sigaction sa;
void signal_handler(int sig, siginfo_t* info, void* ptr);

Jit::Jit() {
    program = nullptr;

    sa.sa_sigaction = signal_handler;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGTRAP, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
}

Jit::~Jit() {
    runtime.release((void*) program);
    delete JitContext::root;
}

bool Jit::load(std::string path) {
    std::string source = readSourceFile(path);

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);

    try {
        JitContext::root = Parse::unit(tokens);
        Scope fileScope;
        JitContext::root->analyze(&fileScope);

        allocateHandles(JitContext::root->functionCount());

        program = Generate::program(&runtime, JitContext::root);

	if (verbose) JitContext::dumpHandles();
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
        std::string source = readSourceFile(path);

        Lexer lexer;
        TokenQueue tokens = lexer.lex(source);

        Unit* newRoot = Parse::unit(tokens);
        newRoot->analyze(fileScope);

        int updated = 0;
        int added = 0;

	if (verbose)
	    std::cout << "Parsed " << newRoot->functions.size() << " functions" << std::endl;
        
        // TODO: O(N^2), will be slow for large files later on! Also not very pretty!
        // (Use some neat tree or map structure for storing functions instead for less expensive lookup)
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

	if (verbose) {
	    std::cout << "New function handles: ";
	    for (FunctionDecl* newFunction : newRoot->functions) {
		std::cout << "[" << newFunction->identifier
			  << " [" << newFunction->bHandleIndex << "]"
			  << "=" << JitContext::handles[newFunction->bHandleIndex]
			  << "] ";
	    }

	    std::cout << std::endl;
	    std::cout << "Added " << added << " function(s)" << std::endl;
	    std::cout << "Updated " << updated << " function(s)" << std::endl;   
	}


    } catch (int i) {
        std::cout << "Runtime error " << i << std::endl;
        return false;
    }

    return true;
}

int Jit::run() {
    return program();
}

void Jit::allocateHandles(unsigned int functionCount) {
    unsigned int capacity = 128 + functionCount * 2;
    JitContext::allocateHandles(capacity);
}

void signal_handler(int sig, siginfo_t* info, void* ptr) {
    ucontext_t* ctxt = (ucontext_t*) ptr;
    void* pc = (void*) ctxt->uc_mcontext.gregs[REG_RIP];

    void* framePtr = __builtin_frame_address(1);

    char* signal = strsignal(sig);
    std::cout << "Program received signal " << BOLD(<<sig<<) << ", " << BOLD(<<signal<<) << std::endl;
    Backtrace::ssd(pc, framePtr);
}





