//
// Created by wax on 2/8/17.
//

#include "Gen.h"

namespace Generate {
    void ifStmt(X86Compiler& c, IfStmt* stmt) {
        Label elseLabel = c.newLabel();
        Label endLabel = c.newLabel();

        X86GpVar* condition = stmt->condition->generate(c);
        c.cmp(*condition, Imm(0));
        c.je(elseLabel);

        stmt->ifBlock->generate(c);

        if (stmt->elseBlock != nullptr) {
            c.jmp(endLabel);
            c.bind(elseLabel);
            stmt->elseBlock->generate(c);
            c.bind(endLabel);
        } else {
            c.bind(elseLabel);
        }
    }
}