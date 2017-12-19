
#include "Expr.h"

TACOp Generate::integerLiteral(TACEnv& env, IntegerLiteral* e) {
    TACType t = TACType(TACKind::SIGNED, 4);
    return env.newImm(t, (unsigned long) e->value);
}
