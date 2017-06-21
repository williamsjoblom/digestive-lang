//
// Created by wax on 1/31/17.
//

#include "Gen.h"

namespace Generate {

    void loop(X86Compiler& c, LoopStmt* stmt) {
        X86Gp condition = stmt->condition->generate(c);

        Label loop = c.newLabel();
        c.bind(loop);

        stmt->body->generate(c);

        c.cmp(condition, Imm(0));
        c.jg(loop); // Loop if condition > 0
    }

}