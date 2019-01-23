#include "BlockStmt.hh"

#include "ast/Stmt.hh"
#include "ir/TACFun.hh"

void Generate::blockStmt(TACFun* env, BlockStmt* stmt) {
    for (Stmt* s : stmt->statements) s->generate(env);
}
