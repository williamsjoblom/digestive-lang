//
// Created by wax on 7/3/17.
//

#ifndef DIG_BASE_H
#define DIG_BASE_H

#include <vector>
#include <asmjit/asmjit.h>

using namespace asmjit;

typedef int (*ProgramType)(void);
typedef std::vector<X86Gp> Regs;

#endif //DIG_TYPES_H
