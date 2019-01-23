#include "IfStmt.hh"

#include <assert.h>

#include "util/PrettyPrint.hh"
#include "genir/IfStmt.hh"


IfStmt::IfStmt(Expr* condition, Stmt* ifBlock) : condition{condition}, ifBlock{ifBlock},
                                                 elseBlock{nullptr} {
    assert(ifBlock != nullptr);
}


IfStmt::IfStmt(Expr* condition, Stmt* ifBlock, Stmt* elseBlock) : condition{condition},
                                                                  ifBlock{ifBlock},
                                                                  elseBlock{elseBlock} {
    assert(ifBlock != nullptr);
}


IfStmt::~IfStmt() {
    delete condition;
    delete ifBlock;
    if (elseBlock != nullptr) delete elseBlock;
}


void IfStmt::analyze(Scope* scope) {
    condition->analyze(scope);

    Scope* ifScope { new Scope(scope) };
    ifBlock->analyze(ifScope);

    if (elseBlock != nullptr) {
        Scope* elseScope { new Scope(scope) };
        elseBlock->analyze(elseScope);
    }
}


void IfStmt::dump(size_t indent) {
    printIndent(indent);

    std::cout << "if ";
    condition->dump();
    std::cout << ":" << std::endl;

    ifBlock->dump(indent);

    if (elseBlock != nullptr) {
        printIndent(indent);
        std::cout << "else ";
        elseBlock->dump(indent);
    }
}


bool IfStmt::equals(const Node& other) const {
    const IfStmt* o { dynamic_cast<const IfStmt*>(&other) };
    if (o == nullptr) return false;

    if (o->elseBlock == nullptr && elseBlock == nullptr) return false;
    if (o->elseBlock == nullptr && elseBlock != nullptr) return false;
    if (o->elseBlock != nullptr && elseBlock == nullptr) return false;

    return *o->condition == *condition && *o->ifBlock == *ifBlock && *o->elseBlock == *elseBlock;
}


void IfStmt::generate(TACFun* env) {
    Generate::ifStmt(env, this);
};
