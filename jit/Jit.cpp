//
// Created by wax on 1/30/17.
//

#include "Jit.h"

#include <fstream>
#include <sstream>
#include <assert.h>

#include "gen/Gen.h"
#include "parse/Parse.h"
#include "lexer/Lexer.h"
#include "JitContext.h"

Jit::Jit() {
    runtime = new JitRuntime();
    program = nullptr;
}

Jit::~Jit() {
    runtime->release((void*) program);
}

bool Jit::load(std::string path) {
    std::string source = readFile(path);

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);

    try {
        root = Parse::unit(tokens);
        Scope* fileScope = new Scope();
        root->analyze(fileScope);

        program = Generate::program(runtime, root);
    } catch (int i) {
        std::cout << "compilation error " << i << std::endl;
        return false;
    }

    return true;
}

bool Jit::reload(std::string path) {
    std::cout << "Reloading '" << path << "'"<< std::endl;
    std::string source = readFile(path);

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);

    try {
        Unit* newRoot = Parse::unit(tokens);
        Scope* fileScope = new Scope();
        root->analyze(fileScope);

        int updated = 0;

        // TODO: O(N^2), will be really slow for large files later on! Also not very pretty!
        for (FunctionDecl* newFunction : newRoot->functions) {
            bool addFunction = true;

            for (int i = 0; i < root->functions.size(); i++) {
                FunctionDecl* oldFunction = root->functions[i];
                if (*newFunction == *oldFunction) {                        // Function not updated.
                    addFunction = false;
                    break;

                } else if (newFunction->matchesSignature(*oldFunction)) {  // Function body updated.
                    newFunction->bHandleIndex = oldFunction->bHandleIndex;
                    root->functions[i] = newFunction;
                    std::cout << "Transfering function handle " << oldFunction->bHandleIndex << std::endl;
                    newFunction->bHandleIndex = oldFunction->bHandleIndex;
                    JitContext::handles[oldFunction->bHandleIndex] = Generate::function(runtime, newFunction);

                    addFunction = false;
                    delete oldFunction;
                    updated++;
                    break;
                }
            }

            if (addFunction) {
                assert(false); // FIXME: Not implemented.
            }
        }

        if (root->functions.size() != this->root->functions.size()) return false;

        for (int i = 0; i < root->functions.size(); i++) {
            Stmt* function =  root->functions[i];
            Stmt* oldFunction = this->root->functions[i];

            if (*function != *oldFunction) return false;
        }

        std::cout << "Updated " << updated << " function(s)" << std::endl;
    } catch (int i) {
        std::cout << "compilation error " << i << std::endl;
        return false;
    }

    return true;
}

std::string Jit::readFile(std::string path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();

    return buffer.str();
}

int Jit::run() {
    return program();
}



