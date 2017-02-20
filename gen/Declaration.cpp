//
// Created by wax on 1/23/17.
//

#include <asmjit/asmjit.h>

#include "ast/VariableDecl.h"

namespace Generate {
    void variableDeclaration(X86Compiler &c, VariableDecl* decl) {
        decl->bVar = c.newInt32(decl->identifier.c_str());
        if (decl->value != nullptr) {
            c.mov(decl->bVar, decl->value->generate(c));
        }
    }
}
