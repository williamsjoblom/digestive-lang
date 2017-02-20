//
// Created by wax on 1/23/17.
//

#include "gen/Gen.h"

namespace Generate {
    void statement(X86Compiler &c, ReturnStmt* stmt) {
        c.ret(stmt->expression->generate(c));
    }
}