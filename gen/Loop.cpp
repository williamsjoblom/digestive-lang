//
// Created by wax on 1/31/17.
//

#include "Gen.h"

namespace Generate {

    void loop(X86Compiler& c, LoopStmt* stmt) {
        Regs condition = stmt->condition->generate(c);
        assert(condition.size() == 1);

        Label loop = c.newLabel();
        c.bind(loop);

        stmt->body->generate(c);

        c.cmp(condition[0], Imm(0));
        c.jg(loop); // Loop if condition > 0
    }

}