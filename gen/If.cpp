//
// Created by wax on 2/8/17.
//

#include "Gen.h"

namespace Generate {
    void ifStmt(X86Compiler& c, IfStmt* stmt) {
        Label elseLabel = c.newLabel();
        Label endLabel = c.newLabel();

        Regs condition = stmt->condition->generate(c);
        assert(condition.size() == 1);
        c.cmp(condition[0], Imm(0));
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