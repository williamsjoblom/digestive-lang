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
