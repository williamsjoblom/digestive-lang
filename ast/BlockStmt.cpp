//
// Created by wax on 12/14/16.
//

#include <vector>
#include "BlockStmt.h"
#include "util/PrettyPrint.h"

BlockStmt::BlockStmt(std::vector<Stmt*> statements) {
    this->statements = statements;
}

void BlockStmt::analyze(Scope* scope) {
    Scope* innerScope = new Scope(scope);
    for (Stmt* stmt : statements) {
        stmt->analyze(innerScope);
    }
}

void BlockStmt::dump(size_t indent) {
    printIndent(indent);
    std::cout << "{" << std::endl;

    for (Stmt* stmt : statements) {
        stmt->dump(indent + 1);
    }

    printIndent(indent);
    std::cout << "}" << std::endl;
}

void BlockStmt::generate(X86Compiler &c) {
    for (Stmt* stmt : statements) {
        stmt->generate(c);
    }
}

