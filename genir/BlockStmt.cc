
#include "BlockStmt.h"

void Generate::blockStmt(TACFun* env, BlockStmt* stmt) {
    for (Stmt* s : stmt->statements) s->generate(env);
}
