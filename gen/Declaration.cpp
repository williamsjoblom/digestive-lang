//
// Created by wax on 1/23/17.
//

#include <asmjit/asmjit.h>

#include "ast/VariableDecl.h"
#include "Gen.h"

namespace Generate {

    void variableDeclaration(X86Compiler &c, VariableDecl* decl) {
        if (decl->value != nullptr) {
            decl->bVar = Generate::cast(c, decl->value, decl->type);
        } else {
            decl->bVar = Generate::typedRegister(c, decl->type);
        }
    }
}
