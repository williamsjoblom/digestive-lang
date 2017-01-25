//
// Created by wax on 12/20/16.
//

#include <assert.h>
#include <util/PrettyPrint.h>
#include "IfStmt.h"

IfStmt::IfStmt(Expr* condition, Stmt* ifBlock) {
    this->condition = condition;
    this->ifBlock = ifBlock;
    this->elseBlock = nullptr;
}

IfStmt::IfStmt(Expr* condition, Stmt* ifBlock, Stmt* elseBlock) {
    this->condition = condition;
    this->ifBlock = ifBlock;
    this->elseBlock = elseBlock;
}

void IfStmt::analyze(Scope *scope) {
    condition->analyze(scope);

    assert(ifBlock != nullptr);
    Scope* ifScope = new Scope(scope);
    ifBlock->analyze(ifScope);

    if (elseBlock != nullptr) {
        Scope* elseScope = new Scope(scope);
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
