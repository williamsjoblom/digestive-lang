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

    void allocateHandles(unsigned int functionCount);
private:
    JitRuntime runtime;
    ProgramType program;
};


#endif //DIG_JIT_H
