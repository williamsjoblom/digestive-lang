//
// Created by wax on 1/24/17.
//
#include "Gen.h"

namespace Generate {

    void unit(X86Compiler &c, Unit* unit) {
        Label l = c.newLabel();
        c.jmp(l);

        for(FunctionDecl* func : unit->functions) {
            func->generate(c);
        }

        c.bind(l);

        c.addFunc(FuncBuilder0<int>(kCallConvHost));

        for(Stmt* stmt : unit->statements) {
            stmt->generate(c);
        }

        c.endFunc();
    }

}