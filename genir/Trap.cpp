#include "Trap.h"


void Generate::trap(TACFun* fun, Trap* stmt) {
    fun->add(TACC::trap, TACOp(), TACOp(), TACOp());
}
