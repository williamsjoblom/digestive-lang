//
// Created by wax on 1/24/17.
//

#include "gen/Gen.h"

Unit::Unit(std::vector<Stmt*> statements, std::vector<FunctionDecl*> functions) {
    this->statements = statements;
    this->functions = functions;
}

void Unit::analyze(Scope *scope) {
    Scope* innerScope = new Scope(scope);

    for (FunctionDecl* func : functions) {
        func->analyze(innerScope);
    }

    for (Stmt* stmt : statements) {
        stmt->analyze(innerScope);
    }
}

void Unit::dump(size_t indent) {
    for (FunctionDecl* func : functions) {
        func->dump(indent + 1);
    }

    for (Stmt* stmt : statements) {
        stmt->dump(indent + 1);
    }
}

void Unit::generate(X86Compiler &c) {
    Generate::unit(c, this);
}

bool Unit::equals(const Node &other) const {
    const Unit* o = dynamic_cast<const Unit*>(&other);
    if (o == nullptr) return false;

    if (o->statements.size() != statements.size()) return false;
    if (o->functions.size() != functions.size()) return false;

    for (int i = 0; i < statements.size(); i++) {
        Stmt* statement =  statements[i];
        Stmt* otherStatement = o->statements[i];

        if (*statement != *otherStatement) return false;
    }

    for (int i = 0; i < functions.size(); i++) {
        Stmt* function =  functions[i];
        Stmt* otherFunction = o->functions[i];

        if (*function != *otherFunction) return false;
    }

    return true;
}
