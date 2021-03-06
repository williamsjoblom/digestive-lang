#include "LoopStmt.hh"

#include <assert.h>

#include "genir/LoopStmt.hh"


LoopStmt::LoopStmt(Expr* condition, Stmt* body) : condition{condition},
                                                  body{body} {
    assert(condition != nullptr);
    assert(body != nullptr);
}


LoopStmt::~LoopStmt() {
    delete condition;
    delete body;
}


void LoopStmt::generate(TACFun* env) {
    Generate::loopStmt(env, this);
}


void LoopStmt::analyze(Scope* scope) {
    Scope* inner { new Scope(scope) };
    condition->analyze(inner);
    body->analyze(inner);
}


bool LoopStmt::equals(const Node& other) const {
    const LoopStmt* o { dynamic_cast<const LoopStmt*>(&other) };
    if (o == nullptr) return false;

    return *o->body == *body && *o->condition == *condition;
}


void LoopStmt::dump(size_t indent) {
    // Not implemented.
}


