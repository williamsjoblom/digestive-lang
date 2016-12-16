//
// Created by wax on 12/15/16.
//

#include "util/PrettyPrint.h"
#include "ReturnStmt.h"

ReturnStmt::ReturnStmt(Expr *expression) {
    this->expression = expression;
}

void ReturnStmt::dump(size_t indent) {
    printIndent(indent);
    std::cout << "return" << std::endl;
    expression->dump(indent + 1);
}


