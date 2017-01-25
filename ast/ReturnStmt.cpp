//
// Created by wax on 12/15/16.
//

#include <gen/Gen.h>
#include "util/PrettyPrint.h"
#include "ReturnStmt.h"

ReturnStmt::ReturnStmt(Expr* expression) {
    this->expression = expression;
}

void ReturnStmt::dump(size_t indent) {
    printIndent(indent);
    std::cout << "return ";
    expression->dump(indent);
    std::cout << std::endl;
}

void ReturnStmt::analyze(Scope* scope) {
    expression->analyze(scope);
}

void ReturnStmt::generate(X86Compiler &c) {
    Generate::statement(c, this);
}


