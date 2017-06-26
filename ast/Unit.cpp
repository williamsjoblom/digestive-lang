//
// Created by wax on 1/24/17.
//

#include <jit/JitContext.h>
#include "gen/Gen.h"

Unit::Unit(std::vector<Stmt*> statements, std::vector<FunctionDecl*> functions) {
    this->statements = new BlockStmt(statements);
    this->functions = functions;
}

Unit::~Unit() {
    delete statements;
    for(FunctionDecl* function : functions) delete function;
}

void Unit::analyze(Scope *scope) {
    Scope* innerScope = new Scope(scope);

    for (FunctionDecl* func : functions) {
        func->analyze(innerScope);
    }

    statements->analyze(innerScope);
}

void Unit::dump(size_t indent) {
    for (FunctionDecl* func : functions) {
        func->dump(indent + 1);
    }

    statements->dump(indent);
}

void Unit::generate(X86Compiler &c) {
    assert(false); // Use Generate::program(...).
}

bool Unit::equals(const Node &other) const {
    const Unit* o = dynamic_cast<const Unit*>(&other);
    if (o == nullptr) return false;

    if(*o->statements != *statements) return false;

    if (o->functions.size() != functions.size()) return false;

    for (int i = 0; i < functions.size(); i++) {
        Stmt* function =  functions[i];
        Stmt* otherFunction = o->functions[i];

        if (*function != *otherFunction) return false;
    }

    return true;
}

unsigned int Unit::functionCount() {
    return (unsigned int)functions.size();
}

std::string Unit::symbolFromPtr(void* ptr) {
    for (FunctionDecl* func : functions) {
        void* ptrLower = JitContext::handles[func->bHandleIndex];
        void* ptrUpper = ptrLower + func->codeSize;

        if (ptr >= ptrLower && ptr <= ptrUpper)
            return func->identifier;
    }

    return "";
}
