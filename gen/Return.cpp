//
// Created by wax on 1/23/17.
//

#include "gen/Gen.h"

namespace Generate {
    void statement(X86Compiler &c, ReturnStmt* stmt) {
        c.ret(Generate::cast(c, stmt->expression, stmt->returnType));
    }
}