#pragma once

#include <string>

#include "ast/Unit.h"
#include "ir/TACProgram.h"
#include "genasm/TACCompiler.h"

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
    TACProgram ir;
    ProgramType program;
};
