//
// Created by wax on 1/30/17.
//

#ifndef DIG_JIT_H
#define DIG_JIT_H

#include <string>

#include "ast/Unit.h"
#include "gen/Gen.h"

class Jit {
public:
    Jit();
    ~Jit();

    bool load(std::string path);
    bool reload(std::string path);
    int run();
private:
    JitRuntime* runtime;
    ProgramType program;
    Unit* root;

    std::string readFile(std::string path);
};


#endif //DIG_JIT_H
